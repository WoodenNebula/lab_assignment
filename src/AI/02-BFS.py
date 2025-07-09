import networkx as nx
import matplotlib.pyplot as plt
from collections import deque

graph = {
    'S': ['A', 'B', 'C'],
    'A': ['D'],
    'B': ['S', 'E'],
    'C': [],
    'D': ['E'],
    'E': []
}
start_node = 'S'

def bfs(graph, start):
    visited_list = set()
    queue = deque([start])
    bfs_order = []

    while queue:
        curr_node = queue.popleft()

        if curr_node not in visited_list:
            visited_list.add(curr_node)
            bfs_order.append(curr_node)

            for neighbor in graph[curr_node]:
                if neighbor not in visited_list:
                    queue.append(neighbor)

    return bfs_order

def save_graph(graph, bfs_order):
    G = nx.DiGraph()

    for node, neighbors in graph.items():
        for neighbor in neighbors:
            G.add_edge(node, neighbor)

    pos = nx.spring_layout(G) 
    node_colors = ['lightgreen' if node in bfs_order else 'lightgray' for node in G.nodes]

    plt.figure(figsize=(8, 6))
    nx.draw(G, pos, with_labels=True, node_color=node_colors, node_size=1200, font_size=16, arrows=True)
    edge_labels = {(u, v): f"{u}->{v}" for u, v in G.edges()}
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='gray')

    plt.title("Graph Visualization with BFS Traversal")
    plt.savefig("02-BFS-Graph.png")


def __main__():
    path = bfs(graph, start_node)
    print("BFS Traversal Order:", path)

    save_graph(graph, path)
    print("\nSurab Parajuli\tSection: A\
            \nRoll: 34\tSymbol: 80010139")

__main__()
