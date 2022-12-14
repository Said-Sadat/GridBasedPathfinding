// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarPathfinding.h"

#include "IndexTypes.h"
#include "TileData.h"

UAStarPathfinding::UAStarPathfinding()
{
}

//Complete pathfinding algorithm using TileData
TArray<FTileData> UAStarPathfinding::AStarPathfinding(FTileData StartNode, FTileData EndNode, TMap<FVector2D, FTileData> gridMap)
{
	TArray<FTileData> Empty;
	TArray<FTileData> ClosedList;
	TArray<FTileData> OpenList;

	GridMap = gridMap;

	StartNode.GCost = 0;
	StartNode.HCost = CalculateHCost(StartNode, EndNode);
	StartNode.FCost = CalculateFCost(StartNode.GCost, StartNode.FCost);
	
	OpenList.Add(StartNode);
	

	while (OpenList.Num() > 0)
	{
		FTileData currentNode = GetLowestFCostNode(OpenList);

		if(currentNode.Index == EndNode.Index)
			return CalculatePath(currentNode);

		OpenList.Remove(currentNode);

		ClosedList.Add(currentNode);

		TArray<FTileData> NeighbourTiles = currentNode.GetTileNeigbours(GridMap,0);
		
		for (auto NeighbourNode : NeighbourTiles)
		{
			NeighbourNode.InitializeNode();
			
			if(ClosedList.Contains(NeighbourNode)) continue;

			float tentativeGCost = currentNode.GCost + CalculateHCost(currentNode, NeighbourNode);
			if(tentativeGCost < NeighbourNode.GCost)
			{
				NeighbourNode.ParentIndex = currentNode.Index;
				NeighbourNode.GCost = tentativeGCost;
				NeighbourNode.HCost = CalculateHCost(NeighbourNode,EndNode);
				NeighbourNode.FCost = CalculateFCost(NeighbourNode.GCost, NeighbourNode.FCost);
				
				GridMap.Add(NeighbourNode.Index, NeighbourNode);
				
				if(!OpenList.Contains(NeighbourNode))
					OpenList.Add(NeighbourNode);
			}
		}
	}

	return Empty;
}

bool UAStarPathfinding::IsDestination(FTileData Node, FTileData EndNode)
{
	FVector2D NodeIndex = Node.Index;
	FVector2D EndNodeIndex = EndNode.Index;
	
	if(NodeIndex == EndNodeIndex)
		return true;
	
	return false;
}

float UAStarPathfinding::CalculateHCost(FTileData Node, FTileData EndNode)
{
	FVector2D NodeIndex = Node.Index;
	FVector2D EndNodeIndex = EndNode.Index;

	float H = FMath::Sqrt(FMath::Square(NodeIndex.X - EndNodeIndex.X) + FMath::Square(NodeIndex.Y - EndNodeIndex.Y));

	return H;
}

float UAStarPathfinding::CalculateFCost(float G, float H)
{
	return G + H;
}

FTileData UAStarPathfinding::GetLowestFCostNode(TArray<FTileData> NodeList)
{
	FTileData lowestFCostNode = NodeList[0];

	for (auto Node : NodeList)
	{
		if(Node.FCost < lowestFCostNode.FCost)
			lowestFCostNode = Node;
	}

	return lowestFCostNode;
}

TArray<FTileData> UAStarPathfinding::CalculatePath(FTileData EndNode)
{
	TArray<FTileData> Path;
	Path.Add(EndNode);
	FTileData currentNode = EndNode;

	while (currentNode.ParentIndex != FVector2D(-100,-100))
	{
		Path.Add(*GridMap.Find(currentNode.ParentIndex));
		currentNode = *GridMap.Find(currentNode.ParentIndex);
	}

	Algo::Reverse(Path);

	return Path;
}
