import networkx as nx
import matplotlib.pyplot as plt
import heapq

graph = {
    'S': ['A', 'B', 'C'],
    'A': ['D'],
    'B': ['S', 'G'],
    'C': [],
    'D': ['G'],
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

def GBFS(graph, heuristic, start, goal):
    visited = set()
    queue = [(heuristic[start], start, [])]  # (h(n), node, path)

    while queue:
        h, node, path = heapq.heappop(queue)
        if node in visited:
            continue

        visited.add(node)
        path = path + [node]

        if node == goal:
            return path

        for neighbor in graph[node]:
            if neighbor not in visited:
                heapq.heappush(queue, (heuristic[neighbor], neighbor, path))

    return None


def save_graph(graph, path):
    G = nx.DiGraph()

    for node, neighbors in graph.items():
        for neighbor in neighbors:
            G.add_edge(node, neighbor)

    pos = nx.spring_layout(G)
    node_colors = ['lightgreen' if node in path else 'lightgray' for node in G.nodes]
    edge_colors = ['red' if u in path and v in path and path.index(v) == path.index(u) + 1 else 'gray'
                   for u, v in G.edges]

    plt.figure(figsize=(8, 6))
    nx.draw(G, pos, with_labels=True, node_color=node_colors,
            edge_color=edge_colors, node_size=1200, font_size=16, arrows=True)

    edge_labels = {(u, v): f"{u}->{v}" for u, v in G.edges()}
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='gray')

    plt.title("Graph Visualization with Greedy Best-First Search")
    plt.savefig("06-GBFS-Graph.png")


def __main__():
    path = GBFS(graph, heuristic, start_node, goal_node)
    save_graph(graph, path)
    
    print(f"GBFS Path from {start_node} to {goal_node}:", path)
    print("\nSurab Parajuli\tSection: A\
            \nRoll: 34\tSymbol: 80010139")

__main__()
