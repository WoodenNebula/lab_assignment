import networkx as nx
import matplotlib.pyplot as plt

graph = {
    'S': ['A', 'B', 'C'],
    'A': ['D'],
    'B': ['S', 'E'],
    'C': [],
    'D': ['E'],
    'E': []
}
start_node = 'S'
depth_limit = 2


def depth_limited_search(graph, start, limit):
    visited = set()
    path = []

    def dls(node, depth):
        if depth > limit:
            return
        visited.add(node)
        path.append(node)

        for neighbor in graph[node]:
            if neighbor not in visited:
                dls(neighbor, depth + 1)

    dls(start, 0)
    return path


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

    plt.title(f"Graph Visualization with DLS (limit={depth_limit})")
    plt.savefig("05-DLS-Graph.png")


def __main__():
    dls_path = depth_limited_search(graph, start_node, depth_limit)

    print(f"DLS Traversal Order up to depth {depth_limit}:", dls_path)
    save_graph(graph, dls_path)

    print("\nSurab Parajuli\tSection: A\
            \nRoll: 34\tSymbol: 80010139")

__main__()
