#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// 窗口大小
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// 最大元件数和节点数
#define MAX_COMPONENTS 100
#define MAX_NODES 100

// 元件类型
#define COMPONENT_TYPE_NONE 0
#define COMPONENT_TYPE_WIRE 1
#define COMPONENT_TYPE_SWITCH 2
#define COMPONENT_TYPE_LED 3
#define COMPONENT_TYPE_RESISTOR 4
#define COMPONENT_TYPE_BATTERY 5
#define COMPONENT_TYPE_AND_GATE 6
#define COMPONENT_TYPE_OR_GATE 7
#define COMPONENT_TYPE_NOT_GATE 8
#define COMPONENT_TYPE_XOR_GATE 9

// 元件结构体
typedef struct {
    int type; // 元件类型
    int node1; // 节点1
    int node2; // 节点2
    double value; // 元件值
    int state; // 元件状态
} Component;

// 节点结构体
typedef struct {
    double voltage; // 节点电压
    int num_components; // 连接该节点的元件数
    int components[MAX_COMPONENTS]; // 连接该节点的元件索引
} Node;

// 全局变量
Component components[MAX_COMPONENTS]; // 元件数组
Node nodes[MAX_NODES]; // 节点数组
int num_components = 0; // 当前元件数
int num_nodes = 0; // 当前节点数
int selected_component = -1; // 当前选中的元件

SDL_Window* window; // SDL窗口
SDL_Renderer* renderer; // SDL渲染器
TTF_Font* font; // 字体

// 初始化SDL
void init_sdl() {
    // 初始化SDL视频子系统
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 初始化SDL_ttf字体子系统
    if (TTF_Init() < 0) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        exit(1);
    }

    // 创建窗口
    window = SDL_CreateWindow("Digital Circuit Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 加载字体
    font = TTF_OpenFont("fonts/DejaVuSans.ttf", 16);
    if (font == NULL) {
        printf("Font could not be loaded! TTF_Error: %s\n", TTF_GetError());
        exit(1);
    }
}

// 释放SDL资源
void deinit_sdl() {
    // 关闭字体
    TTF_CloseFont(font);

    // 销毁渲染器和窗口
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // 退出SDL_ttf和SDL子系统
    TTF_Quit();
    SDL_Quit();
}

// 添加元件
void add_component(int type, int node1, int node2, double value) {
    // 如果当前元件数小于最大元件数,则添加一个新元件
    if (num_components < MAX_COMPONENTS) {
        components[num_components].type = type;
        components[num_components].node1 = node1;
        components[num_components].node2 = node2;
        components[num_components].value = value;
        components[num_components].state = 0;
        num_components++;

        // 更新节点的连接元件
        nodes[node1].components[nodes[node1].num_components++] = num_components - 1;
        nodes[node2].components[nodes[node2].num_components++] = num_components - 1;
    }
}

// 删除元件
void remove_component(int index) {
    // 如果元件索引在有效范围内,则删除该元件
    if (index >= 0 && index < num_components) {
        // 更新节点的连接元件
        for (int i = 0; i < nodes[components[index].node1].num_components; i++) {
            if (nodes[components[index].node1].components[i] == index) {
                nodes[components[index].node1].components[i] = nodes[components[index].node1].components[--nodes[components[index].node1].num_components];
                break;
            }
        }
        for (int i = 0; i < nodes[components[index].node2].num_components; i++) {
            if (nodes[components[index].node2].components[i] == index) {
                nodes[components[index].node2].components[i] = nodes[components[index].node2].components[--nodes[components[index].node2].num_components];
                break;
            }
        }

        // 移动最后一个元件到当前位置
        if (index < num_components - 1) {
            components[index] = components[num_components - 1];
        }
        num_components--;
    }
}

// 设置当前选中的元件
void set_selected_component(int index) {
    selected_component = index;
}

// 更新电路
void update_circuit() {
    // 遍历所有元件,计算节点电压
    for (int i = 0; i < num_nodes; i++) {
        nodes[i].voltage = 0;
        for (int j = 0; j < nodes[i].num_components; j++) {
            int component_index = nodes[i].components[j];
            Component* component = &components[component_index];
            switch (component->type) {
                case COMPONENT_TYPE_WIRE:
                    nodes[component->node1].voltage = nodes[component->node2].voltage;
                    break;
                case COMPONENT_TYPE_SWITCH:
                    if (component->state == 1) {
                        nodes[component->node1].voltage = nodes[component->node2].voltage;
                    } else {
                        nodes[component->node1].voltage = 0;
                    }
                    break;
                case COMPONENT_TYPE_LED:
                    if (nodes[component->node1].voltage > nodes[component->node2].voltage + component->value) {
                        component->state = 1;
                    } else {
                        component->state = 0;
                    }
                    break;
                case COMPONENT_TYPE_RESISTOR:
                    nodes[component->node1].voltage += (nodes[component->node2].voltage - nodes[component->node1].voltage) / component->value;
                    break;
                case COMPONENT_TYPE_BATTERY:
                    nodes[component->node1].voltage = component->value;
                    break;
                case COMPONENT_TYPE_AND_GATE:
                    if (nodes[component->node1].voltage > 0 && nodes[component->node2].voltage > 0) {
                        nodes[component->node1].voltage = component->value;
                    } else {
                        nodes[component->node1].voltage = 0;
                    }
                    break;
                case COMPONENT_TYPE_OR_GATE:
                    if (nodes[component->node1].voltage > 0 || nodes[component->node2].voltage > 0) {
                        nodes[component->node1].voltage = component->value;
                    } else {
                        nodes[component->node1].voltage = 0;
                    }
                    break;
                case COMPONENT_TYPE_NOT_GATE:
                    if (nodes[component->node1].voltage > 0) {
                        nodes[component->node1].voltage = 0;
                    } else {
                        nodes[component->node1].voltage = component->value;
                    }
                    break;
                case COMPONENT_TYPE_XOR_GATE:
                    if ((nodes[component->node1].voltage > 0 && nodes[component->node2].voltage == 0) || (nodes[component->node1].voltage == 0 && nodes[component->node2].voltage > 0)) {
                        nodes[component->node1].voltage = component->value;
                    } else {
                        nodes[component->node1].voltage = 0;
                    }
                    break;
            }
        }
    }
}

// 绘制电路
void draw_circuit() {
    // 清空渲染器
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 绘制元件
    for (int i = 0; i < num_components; i++) {
        Component* component = &components[i];
        int x1 = (nodes[component->node1].voltage / 5.0 + 0.5) * WINDOW_WIDTH;
        int y1 = (nodes[component->node1].voltage / 5.0 + 0.5) * WINDOW_HEIGHT;
        int x2 = (nodes[component->node2].voltage / 5.0 + 0.5) * WINDOW_WIDTH;
        int y2 = (nodes[component->node2].voltage / 5.0 + 0.5) * WINDOW_HEIGHT;

        switch (component->type) {
            case COMPONENT_TYPE_WIRE:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                break;
            case COMPONENT_TYPE_SWITCH:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                if (component->state == 1) {
                    SDL_RenderDrawLine(renderer, x1 - 10, y1 - 10, x1 + 10, y1 + 10);
                    SDL_RenderDrawLine(renderer, x1 - 10, y1 + 10, x1 + 10, y1 - 10);
                }
                break;
            case COMPONENT_TYPE_LED:
                SDL_SetRenderDrawColor(renderer, component->state * 255, component->state * 255, 0, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                SDL_RenderDrawCircle(renderer, x2, y2, 10);
                break;
            case COMPONENT_TYPE_RESISTOR:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x1, y1, (x1 + x2) / 2, y1);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y1, (x1 + x2) / 2, y2);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y2, x2, y2);
                break;
            case COMPONENT_TYPE_BATTERY:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                SDL_RenderDrawLine(renderer, x1 - 10, y1 - 10, x1 + 10, y1 + 10);
                SDL_RenderDrawLine(renderer, x1 - 10, y1 + 10, x1 + 10, y1 - 10);
                break;
            case COMPONENT_TYPE_AND_GATE:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x1, y1, (x1 + x2) / 2, y1);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y1, (x1 + x2) / 2, y2);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y2, x2, y2);
                SDL_RenderDrawLine(renderer, x1 - 10, y1 - 10, x1 + 10, y1 + 10);
                SDL_RenderDrawLine(renderer, x1 - 10, y1 + 10, x1 + 10, y1 - 10);
                break;
            case COMPONENT_TYPE_OR_GATE:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x1, y1, (x1 + x2) / 2, y1);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y1, (x1 + x2) / 2, y2);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y2, x2, y2);
                SDL_RenderDrawArc(renderer, (x1 + x2) / 2, y1, 10, 0, 180);
                break;
            case COMPONENT_TYPE_NOT_GATE:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x1, y1, (x1 + x2) / 2, y1);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y1, (x1 + x2) / 2, y2);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y2, x2, y2);
                SDL_RenderDrawCircle(renderer, x2, y2, 5);
                break;
            case COMPONENT_TYPE_XOR_GATE:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x1, y1, (x1 + x2) / 2, y1);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y1, (x1 + x2) / 2, y2);
                SDL_RenderDrawLine(renderer, (x1 + x2) / 2, y2, x2, y2);
                SDL_RenderDrawArc(renderer, (x1 + x2) / 2, y1, 10, 0, 180);
                SDL_RenderDrawCircle(renderer, x2, y2, 5);
                break;
            }

    // 绘制选中的元件
    if (selected_component != -1) {
        Component* component = &components[selected_component];
        int x1 = (nodes[component->node1].voltage / 5.0 + 0.5) * WINDOW_WIDTH;
        int y1 = (nodes[component->node1].voltage / 5.0 + 0.5) * WINDOW_HEIGHT;
        int x2 = (nodes[component->node2].voltage / 5.0 + 0.5) * WINDOW_WIDTH;
        int y2 = (nodes[component->node2].voltage / 5.0 + 0.5) * WINDOW_HEIGHT;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, (int)((x1 + x2) / 2 - 20), (int)((y1 + y2) / 2 - 20), 40, 40);
    }

    // 更新渲染
    SDL_RenderPresent(renderer);
}

// 绘制波形
void draw_waveform() {
    // 清空渲染器
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 设置绘制颜色为白色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // 绘制网格
    for (int x = 0; x <= WINDOW_WIDTH; x += WINDOW_WIDTH / 10) {
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }
    for (int y = 0; y <= WINDOW_HEIGHT; y += WINDOW_HEIGHT / 10) {
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }

    // 绘制节点电压波形
    for (int i = 0; i < num_nodes; i++) {
        int prev_y = (WINDOW_HEIGHT / 2) - (nodes[i].voltage / 5.0 * (WINDOW_HEIGHT / 2));
        for (int x = 1; x < WINDOW_WIDTH; x++) {
            int y = (WINDOW_HEIGHT / 2) - (nodes[i].voltage / 5.0 * (WINDOW_HEIGHT / 2));
            SDL_RenderDrawLine(renderer, x - 1, prev_y, x, y);
            prev_y = y;
        }
    }

    // 更新渲染
    SDL_RenderPresent(renderer);
}

// 绘制逻辑分析
void draw_logic_analyzer() {
    // 清空渲染器
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 设置绘制颜色为白色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // 绘制网格
    for (int x = 0; x <= WINDOW_WIDTH; x += WINDOW_WIDTH / 10) {
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }
    for (int y = 0; y <= WINDOW_HEIGHT; y += WINDOW_HEIGHT / 10) {
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }

    // 绘制节点电压状态
    for (int i = 0; i < num_nodes; i++) {
        int y = (WINDOW_HEIGHT / num_nodes) * i + WINDOW_HEIGHT / (2 * num_nodes);
        if (nodes[i].voltage > 0) {
            SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
        } else {
            SDL_RenderDrawLine(renderer, 0, y + WINDOW_HEIGHT / (2 * num_nodes), WINDOW_WIDTH, y + WINDOW_HEIGHT / (2 * num_nodes));
        }
    }

    // 更新渲染
    SDL_RenderPresent(renderer);
}

// 绘制电路设计
void draw_circuit_design() {
    // 清空渲染器
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 设置绘制颜色为白色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // 绘制网格
    for (int x = 0; x <= WINDOW_WIDTH; x += WINDOW_WIDTH / 10) {
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }
    for (int y = 0; y <= WINDOW_HEIGHT; y += WINDOW_HEIGHT / 10) {
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }

    // 绘制元件选择菜单
    int menu_x = WINDOW_WIDTH - 200;
    int menu_y = 50;
    SDL_Rect menu_rect = {menu_x, menu_y, 200, WINDOW_HEIGHT - 100};
    SDL_RenderDrawRect(renderer, &menu_rect);
    draw_text(menu_x + 10, menu_y + 10, "Component Selection", (SDL_Color){255, 255, 255, 255});
    draw_text(menu_x + 20, menu_y + 40, "Wire", (SDL_Color){255, 255, 255, 255});
    draw_text(menu_x + 20, menu_y + 60, "Switch", (SDL_Color){255, 255, 255, 255});
    draw_text(menu_x + 20, menu_y + 80, "LED", (SDL_Color){255, 255, 255, 255});
    draw_text(menu_x + 20, menu_y + 100, "Resistor", (SDL_Color){255, 255, 255, 255});
    draw_text(menu_x + 20, menu_y + 120, "Battery", (SDL_Color){255, 255, 255, 255});
    draw_text(menu_x + 20, menu_y + 140, "AND Gate", (SDL_Color){255, 255, 255, 255});
    draw_text(menu_x + 20, menu_y + 160, "OR Gate", (SDL_Color){255, 255, 255, 255});
    draw_text(menu_x + 20, menu_y + 180, "NOT Gate", (SDL_Color){255, 255, 255, 255});
    draw_text(menu_x + 20, menu_y + 200, "XOR Gate", (SDL_Color){255, 255, 255, 255});

    // 更新渲染
    SDL_RenderPresent(renderer);
}

// 绘制信息
void draw_info() {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {255, 0, 0, 255};
    char buffer[256];

    // 绘制当前选中的元件信息
    if (selected_component != -1) {
        Component* component = &components[selected_component];
        sprintf(buffer, "Component Type: %d", component->type);
        draw_text(10, 10, buffer, white);
        sprintf(buffer, "Node 1: %d, Node 2: %d", component->node1, component->node2);
        draw_text(10, 30, buffer, white);
        sprintf(buffer, "Value: %.2f", component->value);
        draw_text(10, 50, buffer, white);
        sprintf(buffer, "State: %d", component->state);
        draw_text(10, 70, buffer, white);
    }

    // 绘制节点电压信息
    for (int i = 0; i < num_nodes; i++) {
        sprintf(buffer, "Node %d: %.2f V", i, nodes[i].voltage);
        draw_text(10, 100 + i * 20, buffer, white);
    }
}

// 绘制文本
void draw_text(int x, int y, const char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

int main(int argc, char* argv[]) {
    // 初始化SDL
    init_sdl();

    // 添加一些初始元件
    add_component(COMPONENT_TYPE_WIRE, 0, 1, 0);
    add_component(COMPONENT_TYPE_SWITCH, 0, 1, 0);
    add_component(COMPONENT_TYPE_LED, 1, 2, 2.0);
    add_component(COMPONENT_TYPE_RESISTOR, 1, 2, 1000.0);
    add_component(COMPONENT_TYPE_BATTERY, 0, 1, 5.0);
    add_component(COMPONENT_TYPE_AND_GATE, 0, 1, 3.3);
    add_component(COMPONENT_TYPE_OR_GATE, 0, 1, 3.3);
    add_component(COMPONENT_TYPE_NOT_GATE, 0, 1, 3.3);
    add_component(COMPONENT_TYPE_XOR_GATE, 0, 1, 3.3);

    SDL_Event event;
    int running = 1;
    int display_mode = 0; // 0 = 电路, 1 = 波形, 2 = 逻辑分析, 3 = 电路设计

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // 检查是否点击了元件
                int x = event.button.x;
                int y = event.button.y;
                for (int i = 0; i < num_components; i++) {
                    int x1 = (nodes[components[i].node1].voltage / 5.0 + 0.5) * WINDOW_WIDTH;
                    int y1 = (nodes[components[i].node1].voltage / 5.0 + 0.5) * WINDOW_HEIGHT;
                    int x2 = (nodes[components[i].node2].voltage / 5.0 + 0.5) * WINDOW_WIDTH;
                    int y2 = (nodes[components[i].node2].voltage / 5.0 + 0.5) * WINDOW_HEIGHT;
                    if (x >= (x1 + x2) / 2 - 20 && x <= (x1 + x2) / 2 + 20 && y >= (y1 + y2) / 2 - 20 && y <= (y1 + y2) / 2 + 20) {
                        set_selected_component(i);
                        break;
                    }
                }
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        add_component(COMPONENT_TYPE_WIRE, num_nodes, num_nodes + 1, 0);
                        break;
                    case SDLK_s:
                        add_component(COMPONENT_TYPE_SWITCH, num_nodes, num_nodes + 1, 0);
                        break;
                    case SDLK_l:
                        add_component(COMPONENT_TYPE_LED, num_nodes, num_nodes + 1, 2.0);
                        break;
                    case SDLK_r:
                        add_component(COMPONENT_TYPE_RESISTOR, num_nodes, num_nodes + 1, 1000.0);
                        break;
                    case SDLK_b:
                        add_component(COMPONENT_TYPE_BATTERY, num_nodes, num_nodes + 1, 5.0);
                        break;
                    case SDLK_a:
                        add_component(COMPONENT_TYPE_AND_GATE, num_nodes, num_nodes + 1, 3.3);
                        break;
                    case SDLK_o:
                        add_component(COMPONENT_TYPE_OR_GATE, num_nodes, num_nodes + 1, 3.3);
                        break;
                    case SDLK_n:
                        add_component(COMPONENT_TYPE_NOT_GATE, num_nodes, num_nodes + 1, 3.3);
                        break;
                    case SDLK_x:
                        add_component(COMPONENT_TYPE_XOR_GATE, num_nodes, num_nodes + 1, 3.3);
                        break;
                    case SDLK_d:
                        remove_component(selected_component);
                        selected_component = -1;
                        break;
                    case SDLK_1:
                        display_mode = 0;
                        break;
                    case SDLK_2:
                        display_mode = 1;
                        break;
                    case SDLK_3:
                        display_mode = 2;
                        break;
                    case SDLK_4:
                        display_mode = 3;
                        break;
                }
            }
        }

        // 更新电路
        update_circuit();

        // 根据显示模式绘制
        switch (display_mode) {
            case 0:
                draw_circuit();
                break;
            case 1:
                draw_waveform();
                break;
            case 2:
                draw_logic_analyzer();
                break;
            case 3:
                draw_circuit_design();
                break;
        }

        // 绘制信息
        draw_info();

        // 延迟10毫秒
        SDL_Delay(10);
    }

    // 释放SDL资源
    deinit_sdl();
    return 0;
}


