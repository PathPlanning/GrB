#pragma once
#include<memory>
#include<map>
#include"cell.h"
#include"utils.h"

class Node;
typedef std::shared_ptr<Node> node_ptr;
#define MAKE_NODE(cell, h) std::make_shared<Node>(cell, h)

class Parent
{
private:
	node_ptr data = nullptr;
	double G;

public:
	Parent(node_ptr n, double g)
	{
		data = n;
		G = g;
	}

	node_ptr get()
	{
		return data;
	}

	double getGToNext()
	{
		return G;
	}

	double getTotalG();
};

typedef std::shared_ptr<Parent> parent_ptr;
#define MAKE_PARENT(n, g) std::make_shared<Parent>(n, g)

class Node
{
private:
	Cell cell;
	parent_ptr bestParent = nullptr;

	std::map<Cell, parent_ptr, CellComparator> prev;
	std::map<Cell, node_ptr, CellComparator> next;

	double G = 0; // all G;
	double H = 0; //to goal

    void updateParent(Cell parentCell)
	{
		auto parentIter = prev.find(parentCell);
		if (parentIter == std::end(prev))
		{
			return;
		}

		auto parent = parentIter->second;
		if (parent->getTotalG() < G)
		{
			bestParent = parent;
			G = parent->getTotalG();
		}
	}

	void findBestParent()
	{
		if (prev.empty())
		{
			bestParent = nullptr;
			return;
		}

		bestParent = begin(prev)->second;

		for (auto iter = std::next(begin(prev)); iter != end(prev); ++iter)
		{
			auto node = iter->second;

			if (node->getTotalG() < bestParent->getTotalG())
			{
				bestParent = node;
			}
		}
	}

	bool isAddPrevAvailable = true;
	bool isAddNextAvailable = true;
public:
	Node()
	{
		G = 0;
		H = 0;
	}

	Node(Cell c, double h) : Node()
	{
		cell = c;
		H = h;
	}

	Node(int x, int y, double h) : Node(Cell(x, y), h)
	{
	}

	void setProtectToAddPrev()
	{
		isAddPrevAvailable = false;
	}

	void setProtectToAddNext()
	{
		isAddNextAvailable = false;
	}

	int operator== (Node &p)
	{
		return cell == p.cell;
	}

	inline node_ptr getBestParent()
	{
		return bestParent != nullptr ? bestParent->get() : nullptr;
	}

    inline Cell& getCell()
	{
		return cell;
	}

	inline double getG()
	{
		return G;
	}

	inline double getH()
	{
		return H;
	}

	inline double getF()
	{
		return G + H;
	}

	inline bool isNextNodeListEmpty()
	{
		return next.empty();
	}

	inline int getNextNodeListSize()
	{
		return next.size();
	}

	inline int getPrevNodeListSize()
	{
		return prev.size();
	}

	inline bool isParentListEmpty()
	{
		return prev.empty();
	}

	inline std::map<Cell, node_ptr, CellComparator> getNext()
	{
		return next;
	}

	void addPrev(node_ptr node, bool& gValueWasUpdated)
	{
		gValueWasUpdated = false;

		if (!isAddPrevAvailable)
		{
			return;
		}

		if (prev.find(node->getCell()) != end(prev))
		{
			return;
		}

		double gToPrev = calculateDistance(cell, node->getCell());

		double totalG = node->getG() + gToPrev;

		auto parent = MAKE_PARENT(node, gToPrev);

		prev.insert(std::make_pair(node->getCell(), parent));

		if (bestParent == nullptr || bestParent->getTotalG() > totalG)
		{
			bestParent = parent;
			G = totalG;

			gValueWasUpdated = true;
		}
	}

	void updateParent(Cell parentCell, bool& gValueWasUpdated)
	{
		gValueWasUpdated = false;

		auto parentIter = prev.find(parentCell);
		if (parentIter == std::end(prev))
		{
			return;
		}

		auto parent = parentIter->second;
		if (parent->getTotalG() < G)
		{
			bestParent = parent;
			G = parent->getTotalG();
			gValueWasUpdated = true;
		}
	}

	void addNext(node_ptr node)
	{
		if (!isAddNextAvailable)
		{
			return;
		}

		if (next.find(node->getCell()) != end(next))
		{
			return;
		}

		next.insert(std::make_pair(node->getCell(), node));
	}

	void removeNext(node_ptr node)
	{
		next.erase(node->getCell());
	}

	void removeParent(node_ptr node)
	{
		prev.erase(node->getCell());

		if (bestParent->get()->getCell() == node->getCell())
		{
			bestParent = nullptr;
			findBestParent();
		}
	}

	std::map<Cell, parent_ptr, CellComparator> getPrevList() 
	{
		return prev;
	}
};
