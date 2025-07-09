import networkx as nx
import matplotlib.pyplot as plt
import heapq

graph = {
    'S': [('A', 2), ('B', 3), ('C', 5)],
    'A': [('C', 1), ('D', 3)],
    'B': [('S', 4), ('E', 2)],
    'C': [],
    'D': [('E', 1)],
    'E': []
}
start_node = 'S'
goal_node = 'E'

def UCS(graph, start, goal=None):
    visited = set()

    # (cost, current_node, path)
    queue = [(0, start, [])] 
    cost, path = None, None
    
    while queue:
        cost, node, path = heapq.heappop(queue)
        if node in visited:
            continue
        visited.add(node)
        path = path + [node]

        if goal and node == goal:
            return path, cost
        
        for neighbor, weight in graph[node]:
            if neighbor not in visited:
                heapq.heappush(queue, (cost + weight, neighbor, path))
    
    return path, cost

def save_graph(graph, path):
    G = nx.DiGraph()

    for node, neighbors in graph.items():
        for neighbor, weight in neighbors:
            G.add_edge(node, neighbor, weight=weight)

    pos = nx.spring_layout(G)
    edge_labels = nx.get_edge_attributes(G, 'weight')

    node_colors = ['lightgreen' if node in path else 'lightgray' for node in G.nodes]
    edge_colors = ['red' if (u in path and v in path and path.index(v) == path.index(u)+1)
                   else 'gray' for u, v in G.edges]

    plt.figure(figsize=(8, 6))
    nx.draw(G, pos, with_labels=True, node_color=node_colors,
            edge_color=edge_colors, node_size=1200, font_size=16, arrows=True)
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='black')

    plt.title("Graph Visualization with UCS Path")
    plt.savefig("03-UFS-Graph.png")


def __main__():
    path, total_cost = UCS(graph, start_node, goal_node)

    print(f"UCS Path from {start_node} to {goal_node}: {path}")
    print(f"Total Cost: {total_cost}")

    save_graph(graph, path)
    print("\nSurab Parajuli\tSection: A\
            \nRoll: 34\tSymbol: 80010139")

__main__()
