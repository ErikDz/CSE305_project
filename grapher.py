import json
import networkx as nx
from pyvis.network import Network

# Function to extract the edge relationships
def extract_edges(urls):
    edges = set()
    for url in urls:
        parts = url.strip('/').split('/')
        if len(parts) > 1:
            for i in range(1, len(parts)):
                parent = '/'.join(parts[:i])
                child = '/'.join(parts[:i+1])
                edges.add((parent, child))
    return list(edges)

# Load URLs from the JSON file
def load_urls_from_json(filepath):
    with open(filepath, 'r') as file:
        urls = json.load(file)
    urls = [entry['url'] for entry in urls]
    return urls

# Main function to create and visualize the graph
def main():
    filepath = 'sitemap.json'
    urls = load_urls_from_json(filepath)

    edges = extract_edges(urls)

    G = nx.DiGraph()
    G.add_edges_from(edges)

    # Create a Pyvis network
    net = Network(notebook=True, height='800px', width='100%', directed=True)
    net.from_nx(G)

    # Customize the physics layout for better visualization
    net.show_buttons(filter_=['physics'])
    net.show('force_directed_graph.html')

if __name__ == "__main__":
    main()
