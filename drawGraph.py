import networkx as nx
import matplotlib.pyplot as plt

# Create a directed graph
G = nx.DiGraph()

# Add edges with weights
edges = [
    ('A', 'B', 3),
    ('A', 'D', 7),
    ('B', 'C', 2),
    ('B', 'E', 5),
    ('C', 'A', 4),
    ('D', 'C', 1),
    ('D', 'E', 6)
]

G.add_weighted_edges_from(edges)

# Positioning the nodes manually or automatically
pos = nx.spring_layout(G)  # or use nx.circular_layout(G) if you prefer

# Draw nodes
nx.draw_networkx_nodes(G, pos, node_size=700)

# Draw edges
nx.draw_networkx_edges(G, pos, arrowstyle='->', arrowsize=20)

# Draw labels
nx.draw_networkx_labels(G, pos, font_size=12, font_family="sans-serif")

# Draw edge weights
edge_labels = {(u, v): d for u, v, d in edges}
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

# Display the plot
plt.axis('off')
plt.tight_layout()
plt.show()