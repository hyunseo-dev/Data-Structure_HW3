#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define INF 1000 		// ���Ѵ� (������ ���� ���)

typedef struct ListNode // ���Ḯ��Ʈ ����ü
{
    int vertex;
    int weight;
    struct ListNode* next;
} ListNode;

typedef struct Graph  // �׷��� ����ü, �����Ϸ��� �׷����� �ϴ� ��������� �Ѵ�.
{
    int num_vertices;
    ListNode** adj_lists;
    int* distance;
    int* found;
    int* prev_vertex;
} Graph;

ListNode* create_node(int v, int w) // ���� �� �Ÿ��� ���� ���ο� ��� ����
{
    ListNode* new_node = malloc(sizeof(ListNode));
    if (new_node == NULL)
    {
        fprintf(stderr, "��� �޸� �Ҵ� ����\n");
        exit(1);
    }
    new_node->vertex = v;
    new_node->weight = w;
    new_node->next = NULL;
    return new_node;
}

Graph* create_graph(int vertices) // ������ ������ �Է¹޾� ������ �˸��� �׷��� ����
{
    Graph* graph = malloc(sizeof(Graph)); // �׷��� ��ü �����Ҵ�
    if (graph == NULL)
    {
        fprintf(stderr, "�׷��� �޸� �Ҵ� ����\n");
        exit(1);
    }
    graph->num_vertices = vertices; // ������ ����
    graph->adj_lists = malloc(vertices * sizeof(ListNode*)); // ���� ����Ʈ ������� �����Ҵ�
    graph->distance = malloc(vertices * sizeof(int)); // �Ÿ� ���� ���� �����Ҵ�
    graph->found = malloc(vertices * sizeof(int)); // found ���� ���� ���� �����Ҵ�
    graph->prev_vertex = malloc(vertices * sizeof(int)); // ���� �湮 ���� ���� ���� �����Ҵ�
    // ��������� ����ü �Ҵ�

    if (graph->adj_lists == NULL || graph->distance == NULL ||
        graph->found == NULL || graph->prev_vertex == NULL)
    {
        fprintf(stderr, "�޸� �Ҵ� ����\n");

        // �Ҵ�� �迭���� �޸� ����
        free(graph->adj_lists);
        free(graph->distance);
        free(graph->found);
        free(graph->prev_vertex);

        // �׷��� ��ü�� �޸� ����
        free(graph);

        exit(1);
    }

    int i;
    for (i = 0; i < vertices; i++) // �� �κк��ʹ� init �뵵�̴�.
    {
        graph->adj_lists[i] = NULL; // �ʱ�ȭ �� ���� ����Ʈ�� ����
        graph->distance[i] = INF; // �Ÿ��� ���Ѵ�� �����ϸ�
        graph->found[i] = FALSE; // �湮�� ��� False�� �����Ѵ�.
        graph->prev_vertex[i] = -1; // ���� ������ �������� �ʴ´�
    } // �׷��� �ؾ� ��������Ʈ�� �߰��Ǹ�, �Ÿ��� ���ŵǰ�, �湮�� True�� �ٲ�� �ǰ�, ���� ������ �߰��ȴ�.

    return graph;
}

void add_vertex(Graph* graph, int src, int dest, int weight) // ���� �߰�
{
    ListNode* new_node = create_node(dest, weight); // ���⼭ ���ο� ��带 �����Ѵ�
    new_node->next = graph->adj_lists[src]; // ���ο� ����� ���� ��带 ���� ����Ʈ�� ù ��° ���� ����
    graph->adj_lists[src] = new_node; // source�� ���� ������ ���ο� ���
}

int choose(Graph* graph) // ���� �ڵ� Ȱ��
{
    int min = INF; // INT_MAX ��� ����Ͽ��� �� �۵��ϴ� ������ ���� ����߽��ϴ�. �ڵ� ���ذ� ���� �� ���� ������ INF�� ����߽��ϴ�.
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

void shortest_path(Graph* graph, int start_vertex) // ���� �ڵ� Ȱ��
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
                    graph->prev_vertex[temp->vertex] = u; // ���� ���� �߰�
                    // ���� print_path���� ������ ������ ���
                }
            }
            temp = temp->next;
        }
    }
}

void print_path(Graph* graph, int start_vertex, int end_vertex) // ��� ��� �Լ�
{
    if (graph->prev_vertex[end_vertex] == -1) // ���� ������ ���ٸ� ��δ� �������� �ʴ´�. �ʱ�ȭ�� -1�� �Ǿ��ְ�,
                                              //���� ���ο� ���� ������ �ʴ´ٸ� -1�� ���´�
    {
        printf("%d�κ��� %d�� ��δ� �������� �ʽ��ϴ�.\n", start_vertex, end_vertex);
        return;
    }

    int* path = malloc(graph->num_vertices * sizeof(int)); // ���
    int* weights = malloc(graph->num_vertices * sizeof(int)); // ����ġ ���� �Ҵ�
    if (path == NULL || weights == NULL)
    {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        if (path != NULL)
            free(path);
        if (weights != NULL)
            free(weights);
        exit(1);
    }
    int path_length = 0;
    int vertex = end_vertex; // ���������� Ž��
    while (vertex != -1) // ���� ������ ���� ��� ����
    {
        path[path_length] = vertex; // ���� ���,
        weights[path_length] = graph->distance[vertex]; // ����ġ ������ ����
        path_length++; // ���� �迭�� �̵�
        vertex = graph->prev_vertex[vertex]; // ���� ���� �������� �̵�
    }

    printf("%d�κ��� %d������ �ִ� ���: ", start_vertex, end_vertex); // �������� Ž�������Ƿ� �ݴ�� ���
    for (int i = path_length - 1; i >= 0; i--)
    {
        printf("%d", path[i]);
        if (i > 0)
            printf(" -> ");
    }
    printf("\n");

    printf("�ִ� ��α����� ����ġ ��ȭ ����: "); // ����ġ�� ��ȭ ���� Ž�� �� ���
    for (int i = path_length - 1; i >= 0; i--)
    {
        printf("%d", weights[i]);
        if (i > 0)
            printf(" -> ");
    }
    printf("\n");
    printf("�ִ� ��α����� ���� ����ġ = %d\n", graph->distance[end_vertex]);
    printf("\n");

    free(path);
    free(weights);
}



void main()
{
    Graph* graph = create_graph(7); // �׷��� ����
    // ���� �߰�
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

    // �ִ� �Ÿ� ��� ��, ��� ���
    shortest_path(graph, 0);
    for (int end_vertex = 0; end_vertex < graph->num_vertices; end_vertex++)
    {
        if (end_vertex != 0)
        {
            print_path(graph, 0, end_vertex);
        }
    }
}