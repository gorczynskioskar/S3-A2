// ALGO2 IS1 211B LAB08
// Oskar Górczyñski
// go57785@zut.edu.pl

#include <iostream>
#include <fstream>
#include <string>

template <typename T> class linkedList {
private:
	struct Node {
		T data;
		Node* next;
		Node* prev;
	};
	Node* head = nullptr;
	Node* tail = nullptr;
	size_t length = 0;
	int(*compare)(const T&, const T&);
public:
	linkedList(int(*cmp)(const T&, const T&) = nullptr) {
		compare = cmp;
	}
	size_t getLength() {
		return length;
	}
	void addTail(T object) {
		auto newTail = new Node;
		newTail->data = object;
		newTail->next = nullptr;
		if (length == 0) {
			newTail->prev = nullptr;
			head = newTail;
		}
		else {
			tail->next = newTail;
			newTail->prev = tail;
		}
		tail = newTail;
		length++;
		return;
	}
	T popHead() {
		if (length == 0) throw std::out_of_range("List is empty");
		T object = head->data;
		Node* oldHead = head;
		head = head->next;
		delete oldHead;
		length--;
		if (length == 0) {
			tail = nullptr;
		}
		return object;
	}
	void insertionSort(int(*cmp)(const T&, const T&)) {
		if (length < 2) return;
		Node* current = head->next;
		while (current != nullptr) {
			T key = current->data;
			Node* prevNode = current->prev;
			while (prevNode != nullptr && cmp(prevNode->data, key) > 0) {
				prevNode->next->data = prevNode->data;
				prevNode = prevNode->prev;
			}
			if (prevNode == nullptr) {
				head->data = key;
			}
			else {
				prevNode->next->data = key;
			}
			current = current->next;
		}
	}
	void delTail() {
		if (length == 0) {
			std::cout << "Lista jest pusta!";
			return;
		}
		else if (length == 1) {
			delete tail;
			head = nullptr;
			tail = nullptr;
		}
		else {
			tail->prev->next = nullptr;
			auto temp = tail;
			tail = tail->prev;
			delete temp;
		}
		length--;
		return;
	}
	void clearList() {
		size_t len = getLength();
		for (unsigned i = 0; i < len; i++) {
			delTail();
		}
	}
	void display() {
		Node* current = head;
		while (current != nullptr) {
			std::cout << current->data.from << " -> " << current->data.to << " (weight: " << current->data.weight << ")" << std::endl;
			current = current->next;
		}
	}
	template <typename Func>
	void forEach(Func f) const {
		Node* current = head;
		while (current != nullptr) {
			f(current->data);
			current = current->next;
		}
	}
};
template <typename T> void bucket_sort(T* data, unsigned int size, int(*cmp)(const T&, const T&)) {
	const int No_OF_BUCKETS = size;
	linkedList<T>* buckets = new linkedList<T>[No_OF_BUCKETS];
	for (int i = 0; i < No_OF_BUCKETS; i++) {
		buckets[i] = linkedList<T>(cmp);
	}
	for (int i = 0; i < size; i++) {
		int bucketIndex = data[i].weight * No_OF_BUCKETS;
		buckets[bucketIndex].addTail(data[i]);
	}
	for (int i = 0; i < No_OF_BUCKETS; i++) {
		buckets[i].insertionSort(cmp);
	}
	int j = 0;
	for (int i = 0; i < No_OF_BUCKETS; i++) {
		while (buckets[i].getLength() > 0) {
			data[j++] = buckets[i].popHead();
		}
	}
	delete[] buckets;
}

struct GraphNode {
	double x;
	double y;
};
struct GraphEdge {
	int from;
	int to;
	double weight;
};
#include <iomanip>
struct DotEdgeWriter {
	std::ostream& out;
	DotEdgeWriter(std::ostream& o) : out(o) {}

	void operator()(const GraphEdge& e) const {
		out << "  " << e.from << " -- " << e.to
			<< " [label=\"" << std::fixed << std::setprecision(3)
			<< e.weight << "\"];\n";
	}
};

int EdgeCompare(const GraphEdge& a, const GraphEdge& b) {
	if (a.weight < b.weight) return -1;
	if (a.weight > b.weight) return 1;
	return 0;
}
struct Graph {
	int numNodes;
	int numEdges;
	unsigned int maxNodes;
	unsigned int maxEdges;
	GraphNode* nodes;
	GraphEdge* edges;
	Graph() {
		numNodes = 0;
		numEdges = 0;
		maxNodes = 0;
		maxEdges = 0;
		nodes = nullptr;
		edges = nullptr;
	}
	~Graph() {
		delete[] nodes;
		delete[] edges;
	}
	void addNode(double x, double y) {
		if (numNodes < maxNodes) {
			nodes[numNodes].x = x;
			nodes[numNodes].y = y;
			numNodes++;
			return;
		}
		GraphNode* temp = new GraphNode[maxNodes * 2];
		for (unsigned int i = 0; i < maxNodes; ++i) {
			temp[i] = nodes[i];
		}
		delete[] nodes;
		nodes = temp;
		maxNodes *= 2;
		nodes[numNodes].x = x;
		nodes[numNodes].y = y;
		numNodes++;
	}
	void addEdge(int from, int to, double weight) {
		if (numEdges < maxEdges) {
			edges[numEdges].from = from;
			edges[numEdges].to = to;
			edges[numEdges].weight = weight;
			numEdges++;
			return;
		}
		GraphEdge* temp = new GraphEdge[maxEdges * 2];
		for (unsigned int i = 0; i < maxEdges; ++i) {
			temp[i] = edges[i];
		}
		delete[] edges;
		edges = temp;
		maxEdges *= 2;
		edges[numEdges].from = from;
		edges[numEdges].to = to;
		edges[numEdges].weight = weight;
		numEdges++;
	}
	void importFromTxt(const char* filename) {
		std::ifstream file(filename, std::ios::in);
		if (!file.is_open()) {
			std::cerr << "Error opening file: " << filename << std::endl;
			return;
		}
		file >> maxNodes;
		nodes = new GraphNode[maxNodes];
		for (unsigned int i = 0; i < maxNodes; ++i) {
			double x, y;
			file >> x >> y;
			addNode(x, y);
		}
		file >> maxEdges;
		edges = new GraphEdge[maxEdges];
		for (unsigned int i = 0; i < maxEdges; ++i) {
			int from, to;
			double weight;
			file >> from >> to >> weight;
			addEdge(from, to, weight);
		}
		file.close();
	}
	void sortEdges() {
		bucket_sort<GraphEdge>(edges, numEdges, EdgeCompare);
	}
};

void exportMSTtoDOT(const Graph& graph,
	const linkedList<GraphEdge>& mst,
	const std::string& filename)
{
	std::ofstream out(filename);
	if (!out.is_open()) {
		std::cerr << "Unable to save file: " << filename << "\n";
		return;
	}

	out << "graph MST {\n";
	out << "  overlap=false;\n";
	out << "  splines=true;\n";
	out << "  node [shape=circle, fontsize=10];\n";
	out << "  edge [fontsize=9];\n";
	out << "  layout=neato;\n";

	for (int i = 0; i < graph.numNodes; ++i) {
		out << "  " << i << " [pos=\""
			<< std::fixed << std::setprecision(3)
			<< graph.nodes[i].x << "," << graph.nodes[i].y << "!\"];\n";
	}
	mst.forEach(DotEdgeWriter(out));
	out << "}\n";
}

struct UnionFind {
	int* parent;
	int* rank;
	bool withCompression;
	bool byRank;
	UnionFind(int n, bool useCompression, bool uniteByRank) {
		parent = new int[n];
		rank = new int[n];
		withCompression = useCompression;
		byRank = uniteByRank;
		for (int i = 0; i < n; ++i) {
			parent[i] = i;
			rank[i] = 0;
		}
	}
	~UnionFind() {
		delete[] parent;
		delete[] rank;
	}
	int find(int u, unsigned int& referenceCounter) {
		referenceCounter++;
		if (parent[u] == u) return u;
		if (withCompression) {
			int root = find(parent[u], referenceCounter);
			if (root != parent[u]) {
				parent[u] = root;
			}
			return root;
		}
		else return find(parent[u], referenceCounter);
	}

	bool unite(int u, int v, unsigned int& referenceCounter) {
		int rootU = find(u, referenceCounter);
		int rootV = find(v, referenceCounter);
		if (rootU != rootV) {
			if (byRank) {
				if (rank[rootU] > rank[rootV]) {
					parent[rootV] = rootU;
				}
				else if (rank[rootU] < rank[rootV]) {
					parent[rootU] = rootV;
				}
				else {
					parent[rootV] = rootU;
					rank[rootU]++;
				}
			}
			else {
				parent[rootV] = rootU;
			}
			return true;
		}
		return false;
	}
};
linkedList<GraphEdge> Kruskal(Graph& graph, unsigned int& NoOfEdges, double& sumOfWeights, long& sortDuration, long& mainKruskalLoopDuration, unsigned int& NoOfFindOperations, bool usePathCompression, bool uniteByRank) {
	UnionFind uf(graph.numNodes, usePathCompression, uniteByRank);
	clock_t start = clock();
	graph.sortEdges();
	clock_t end = clock();
	sortDuration = (double)(end - start) / CLOCKS_PER_SEC * 1000; //ms
	linkedList<GraphEdge> mstEdges;
	start = clock();
	for (int i = 0; i < graph.numEdges; ++i) {
		GraphEdge edge = graph.edges[i];
		if (uf.unite(edge.from, edge.to, NoOfFindOperations)) {
			mstEdges.addTail(edge);
			sumOfWeights += edge.weight;
			NoOfEdges++;
			if (NoOfEdges == graph.numNodes - 1) break;
		}
	}
	end = clock();
	mainKruskalLoopDuration = (end - start) * 1000. / CLOCKS_PER_SEC; //ms
	return mstEdges;
}

int main() {
	//g1 pc-on, ubr-on
	unsigned int NoOfEdges = 0;
	double sumOfWeights = 0;
	long sortDuration = 0;
	long mainKruskalLoopDuration = 0;
	unsigned int NoOfFindOperations = 0;
	Graph graph11;
	graph11.importFromTxt("g1.txt");
	graph11.sortEdges();
	linkedList<GraphEdge> mstEdges11 = Kruskal(graph11, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, true, true);
	std::cout << "Statistics for file 'g1.txt':\n";
	std::cout << "\nUnion by rank: ON";
	std::cout << "\nPath compression: ON";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	//g1 pc-on, ubr-off
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph12;
	graph12.importFromTxt("g1.txt");
	graph12.sortEdges();
	linkedList<GraphEdge> mstEdges12 = Kruskal(graph12, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, true, false);
	std::cout << "\n\nUnion by rank: OFF";
	std::cout << "\nPath compression: ON";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	//g1 pc-off, ubr-on
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph13;
	graph13.importFromTxt("g1.txt");
	graph13.sortEdges();
	linkedList<GraphEdge> mstEdges13 = Kruskal(graph13, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, false, true);
	std::cout << "\n\nUnion by rank: ON";
	std::cout << "\nPath compression: OFF";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	//g1 pc-off, ubr-off
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph14;
	graph14.importFromTxt("g1.txt");
	graph14.sortEdges();
	linkedList<GraphEdge> mstEdges14 = Kruskal(graph14, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, false, false);
	std::cout << "\n\nUnion by rank: OFF";
	std::cout << "\nPath compression: OFF";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	std::cout << "\n=======================================================\n";
	//g2 pc-on, ubr-on
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph21;
	graph21.importFromTxt("g2.txt");
	graph21.sortEdges();
	linkedList<GraphEdge> mstEdges21 = Kruskal(graph21, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, true, true);
	std::cout << "\nStatistics for file 'g2.txt':\n";
	std::cout << "\nUnion by rank: ON";
	std::cout << "\nPath compression: ON";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	//g2 pc-on, ubr-off
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph22;
	graph22.importFromTxt("g2.txt");
	graph22.sortEdges();
	linkedList<GraphEdge> mstEdges22 = Kruskal(graph22, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, true, false);
	std::cout << "\n\nUnion by rank: OFF";
	std::cout << "\nPath compression: ON";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	//g2 pc-off, ubr-on
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph23;
	graph23.importFromTxt("g2.txt");
	graph23.sortEdges();
	linkedList<GraphEdge> mstEdges23 = Kruskal(graph23, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, false, true);
	std::cout << "\n\nUnion by rank: ON";
	std::cout << "\nPath compression: OFF";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	//g2 pc-off, ubr-off
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph24;
	graph24.importFromTxt("g2.txt");
	graph24.sortEdges();
	linkedList<GraphEdge> mstEdges24 = Kruskal(graph24, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, false, false);
	std::cout << "\n\nUnion by rank: OFF";
	std::cout << "\nPath compression: OFF";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	std::cout << "\n=======================================================\n";
	//g3 pc-on, ubr-on
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph31;
	graph31.importFromTxt("g3.txt");
	graph31.sortEdges();
	linkedList<GraphEdge> mstEdges31 = Kruskal(graph31, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, true, true);
	std::cout << "\nStatistics for file 'g3.txt':\n";
	std::cout << "\nUnion by rank: ON";
	std::cout << "\nPath compression: ON";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	//g3 pc-on, ubr-off
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph32;
	graph32.importFromTxt("g3.txt");
	graph32.sortEdges();
	linkedList<GraphEdge> mstEdges32 = Kruskal(graph32, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, true, false);
	std::cout << "\n\nUnion by rank: OFF";
	std::cout << "\nPath compression: ON";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	//g3 pc-off, ubr-on
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph33;
	graph33.importFromTxt("g3.txt");
	graph33.sortEdges();
	linkedList<GraphEdge> mstEdges33 = Kruskal(graph33, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, false, true);
	std::cout << "\n\nUnion by rank: ON";
	std::cout << "\nPath compression: OFF";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	//g3 pc-off, ubr-off
	NoOfEdges = 0;
	sumOfWeights = 0;
	sortDuration = 0;
	mainKruskalLoopDuration = 0;
	NoOfFindOperations = 0;
	Graph graph34;
	graph34.importFromTxt("g3.txt");
	graph34.sortEdges();
	linkedList<GraphEdge> mstEdges34 = Kruskal(graph34, NoOfEdges, sumOfWeights, sortDuration, mainKruskalLoopDuration, NoOfFindOperations, false, false);
	std::cout << "\n\nUnion by rank: OFF";
	std::cout << "\nPath compression: OFF";
	std::cout << "\nNo. of edges: " << NoOfEdges;
	std::cout << "\nSum of weights: " << sumOfWeights;
	std::cout << "\nSort duration: " << sortDuration << " ms.";
	std::cout << "\nMain loop duration: " << mainKruskalLoopDuration << " ms.";
	std::cout << "\nNo. of find operations: " << NoOfFindOperations;
	std::cout << "\n=======================================================\n";
	exportMSTtoDOT(graph11, mstEdges11, "mst_g1.dot");
	exportMSTtoDOT(graph21, mstEdges21, "mst_g2.dot");
	exportMSTtoDOT(graph31, mstEdges31, "mst_g3.dot");
	return 0;
}