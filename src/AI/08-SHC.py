import networkx as nx
import matplotlib.pyplot as plt

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


def steepest_hill_climb(graph, heuristic, start, goal):
    current = start
    path = [current]

    while current != goal:
        neighbors = graph[current]
        if not neighbors:
            break  

        # Choose neighbor with lowest heuristic
        next_node = min(neighbors, key=lambda n: heuristic[n])

        # If it's worse or equal, we're stuck
        if heuristic[next_node] >= heuristic[current]:
            break

        current = next_node
        path.append(current)

    return path if current == goal else None


def save_graph(graph, path):
    G = nx.DiGraph()

    for node, neighbors in graph.items():
        for neighbor in neighbors:
            G.add_edge(node, neighbor)

    pos = nx.spring_layout(G)
    node_colors = ['lightgreen' if path and node in path else 'lightgray' for node in G.nodes]
    edge_colors = ['red' if path and u in path and v in path and path.index(v) == path.index(u) + 1 else 'gray'
                   for u, v in G.edges]

    plt.figure(figsize=(8, 6))
    nx.draw(G, pos, with_labels=True, node_color=node_colors,
            edge_color=edge_colors, node_size=1200, font_size=16, arrows=True)

    edge_labels = {(u, v): f"{u}->{v}" for u, v in G.edges()}
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='gray')

    plt.title("Graph Visualization with Steepest Hill Climbing")
    plt.savefig("08-SHC-Graph.png")

def __main__():
    hill_path = steepest_hill_climb(graph, heuristic, start_node, goal_node)

    if hill_path:
        print(f"Hill Climbing Path from {start_node} to {goal_node}: {hill_path}")
    else:
        print(f"Could not reach {goal_node} from {start_node} using Hill Climbing.")

    print("\nSurab Parajuli\tSection: A\
            \nRoll: 34\tSymbol: 80010139")

__main__()
