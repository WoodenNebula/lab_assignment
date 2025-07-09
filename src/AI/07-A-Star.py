import networkx as nx
import matplotlib.pyplot as plt
import heapq

graph = {
    'S': [('A', 2), ('B', 3), ('C', 5)],
    'A': [('C', 1), ('D', 3)],
    'B': [('S', 4), ('G', 2)],
    'C': [],
    'D': [('G', 1)],
    'G': []
}
start_node = 'S'
goal_node = 'G'

heuristic = {
    'S': 6,
    'A': 4,
    'B': 3,
    'C': 7,
    'D': 2,
    'G': 1
}

def a_star(graph, heuristic, start, goal):
    open_set = [(heuristic[start], 0, start, [])]  
    visited = {}

    while open_set:
        f, g, node, path = heapq.heappop(open_set)

        if node in visited and visited[node] <= g:
            continue

        visited[node] = g
        path = path + [node]

        if node == goal:
            # path and total cost
            return path, g

        for neighbor, cost in graph[node]:
            g_new = g + cost
            f_new = g_new + heuristic[neighbor]
            heapq.heappush(open_set, (f_new, g_new, neighbor, path))

    return None, float('inf')


def save_graph(graph, path):
    G = nx.DiGraph()

    for node, neighbors in graph.items():
        for neighbor, weight in neighbors:
            G.add_edge(node, neighbor, weight=weight)

    pos = nx.spring_layout(G)
    node_colors = ['lightgreen' if node in path else 'lightgray' for node in G.nodes]
    edge_colors = ['red' if u in path and v in path and path.index(v) == path.index(u) + 1 else 'gray'
                   for u, v in G.edges]

    plt.figure(figsize=(8, 6))
    nx.draw(G, pos, with_labels=True, node_color=node_colors,
            edge_color=edge_colors, node_size=1200, font_size=16, arrows=True)

    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='black')

    plt.title("Graph Visualization with A* Search Path")
    plt.savefig("07-A-Star-Graph.png")


def __main__():
    path, total_cost = a_star(graph, heuristic, start_node, goal_node)

    save_graph(graph, path)
    print(f"A* Path from {start_node} to {goal_node}: {path}")
    print(f"Total Cost: {total_cost}")

    print("\nSurab Parajuli\tSection: A\
            \nRoll: 34\tSymbol: 80010139")

__main__()
