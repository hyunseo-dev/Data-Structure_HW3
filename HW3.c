#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define INF 1000 		// 무한대 (연결이 없는 경우)

typedef struct ListNode // 연결리스트 구조체
{
    int vertex;
    int weight;
    struct ListNode* next;
} ListNode;

typedef struct Graph  // 그래프 구조체, 동작하려면 그래프를 일단 생성해줘야 한다.
{
    int num_vertices;
    ListNode** adj_lists;
    int* distance;
    int* found;
    int* prev_vertex;
} Graph;

ListNode* create_node(int v, int w) // 정점 간 거리에 따른 새로운 노드 생성
{
    ListNode* new_node = malloc(sizeof(ListNode));
    if (new_node == NULL)
    {
        fprintf(stderr, "노드 메모리 할당 오류\n");
        exit(1);
    }
    new_node->vertex = v;
    new_node->weight = w;
    new_node->next = NULL;
    return new_node;
}

Graph* create_graph(int vertices) // 정점의 개수를 입력받아 개수에 알맞은 그래프 생성
{
    Graph* graph = malloc(sizeof(Graph)); // 그래프 자체 동적할당
    if (graph == NULL)
    {
        fprintf(stderr, "그래프 메모리 할당 오류\n");
        exit(1);
    }
    graph->num_vertices = vertices; // 정점의 개수
    graph->adj_lists = malloc(vertices * sizeof(ListNode*)); // 인접 리스트 저장공간 동적할당
    graph->distance = malloc(vertices * sizeof(int)); // 거리 저장 공간 동적할당
    graph->found = malloc(vertices * sizeof(int)); // found 변수 저장 공간 동적할당
    graph->prev_vertex = malloc(vertices * sizeof(int)); // 이전 방문 정점 저장 공간 동적할당
    // 여기까지가 구조체 할당

    if (graph->adj_lists == NULL || graph->distance == NULL ||
        graph->found == NULL || graph->prev_vertex == NULL)
    {
        fprintf(stderr, "메모리 할당 오류\n");

        // 할당된 배열들의 메모리 해제
        free(graph->adj_lists);
        free(graph->distance);
        free(graph->found);
        free(graph->prev_vertex);

        // 그래프 자체의 메모리 해제
        free(graph);

        exit(1);
    }

    int i;
    for (i = 0; i < vertices; i++) // 이 부분부터는 init 용도이다.
    {
        graph->adj_lists[i] = NULL; // 초기화 시 인접 리스트는 비우고
        graph->distance[i] = INF; // 거리는 무한대로 설정하며
        graph->found[i] = FALSE; // 방문은 모두 False로 설정한다.
        graph->prev_vertex[i] = -1; // 이전 정점은 존재하지 않는다
    } // 그렇게 해야 인접리스트는 추가되며, 거리는 갱신되고, 방문은 True로 바뀌게 되고, 이전 정점이 추가된다.

    return graph;
}

void add_vertex(Graph* graph, int src, int dest, int weight) // 정점 추가
{
    ListNode* new_node = create_node(dest, weight); // 여기서 새로운 노드를 생성한다
    new_node->next = graph->adj_lists[src]; // 새로운 노드의 다음 노드를 현재 리스트의 첫 번째 노드로 연결
    graph->adj_lists[src] = new_node; // source의 다음 정점은 새로운 노드
}

int choose(Graph* graph) // 기존 코드 활용
{
    int min = INF; // INT_MAX 대신 사용하여도 잘 작동하는 것으로 보여 사용했습니다. 코드 이해가 조금 더 쉽게 느껴져 INF로 사용했습니다.
    int minpos = -1;
    for (int i = 0; i < graph->num_vertices; i++)
    {
        if (graph->distance[i] < min && !graph->found[i])
        {
            min = graph->distance[i];
            minpos = i;
        }
    }
    return minpos;
}

void shortest_path(Graph* graph, int start_vertex) // 기존 코드 활용
{
    ListNode* temp;
    graph->distance[start_vertex] = 0;
    int u;
    for (int i = 0; i < graph->num_vertices - 1; i++)
    {
        u = choose(graph);
        graph->found[u] = TRUE;
        temp = graph->adj_lists[u];
        while (temp)
        {
            if (!graph->found[temp->vertex])
            {
                if (graph->distance[u] + temp->weight < graph->distance[temp->vertex])
                {
                    graph->distance[temp->vertex] = graph->distance[u] + temp->weight;
                    graph->prev_vertex[temp->vertex] = u; // 이전 정점 추가
                    // 추후 print_path에서 정점을 추적해 사용
                }
            }
            temp = temp->next;
        }
    }
}

void print_path(Graph* graph, int start_vertex, int end_vertex) // 경로 출력 함수
{
    if (graph->prev_vertex[end_vertex] == -1) // 이전 정점이 없다면 경로는 존재하지 않는다. 초기화가 -1로 되어있고,
                                              //이후 새로운 값이 들어오지 않는다면 -1로 남는다
    {
        printf("%d로부터 %d의 경로는 존재하지 않습니다.\n", start_vertex, end_vertex);
        return;
    }

    int* path = malloc(graph->num_vertices * sizeof(int)); // 경로
    int* weights = malloc(graph->num_vertices * sizeof(int)); // 가중치 공간 할당
    if (path == NULL || weights == NULL)
    {
        fprintf(stderr, "메모리 할당 오류\n");
        if (path != NULL)
            free(path);
        if (weights != NULL)
            free(weights);
        exit(1);
    }
    int path_length = 0;
    int vertex = end_vertex; // 끝에서부터 탐색
    while (vertex != -1) // 이전 정점이 없을 경우 종료
    {
        path[path_length] = vertex; // 각각 경로,
        weights[path_length] = graph->distance[vertex]; // 가중치 정보를 저장
        path_length++; // 다음 배열로 이동
        vertex = graph->prev_vertex[vertex]; // 다음 역순 정점으로 이동
    }

    printf("%d로부터 %d까지의 최단 경로: ", start_vertex, end_vertex); // 역순으로 탐색했으므로 반대로 출력
    for (int i = path_length - 1; i >= 0; i--)
    {
        printf("%d", path[i]);
        if (i > 0)
            printf(" -> ");
    }
    printf("\n");

    printf("최단 경로까지의 가중치 변화 과정: "); // 가중치의 변화 역순 탐색 후 출력
    for (int i = path_length - 1; i >= 0; i--)
    {
        printf("%d", weights[i]);
        if (i > 0)
            printf(" -> ");
    }
    printf("\n");
    printf("최단 경로까지의 최종 가중치 = %d\n", graph->distance[end_vertex]);
    printf("\n");

    free(path);
    free(weights);
}



void main()
{
    Graph* graph = create_graph(7); // 그래프 생성
    // 정점 추가
    add_vertex(graph, 0, 1, 7);
    add_vertex(graph, 0, 4, 3);
    add_vertex(graph, 0, 5, 10);
    add_vertex(graph, 1, 2, 4);
    add_vertex(graph, 1, 3, 10);
    add_vertex(graph, 1, 4, 2);
    add_vertex(graph, 1, 5, 6);
    add_vertex(graph, 2, 3, 2);
    add_vertex(graph, 3, 4, 11);
    add_vertex(graph, 3, 5, 9);
    add_vertex(graph, 3, 6, 4);
    add_vertex(graph, 4, 6, 5);

    // 최단 거리 계산 후, 결과 출력
    shortest_path(graph, 0);
    for (int end_vertex = 0; end_vertex < graph->num_vertices; end_vertex++)
    {
        if (end_vertex != 0)
        {
            print_path(graph, 0, end_vertex);
        }
    }
}