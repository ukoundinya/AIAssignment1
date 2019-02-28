#include "Grid.h"
#include "ofBitmapFont.h"

void Grid::Load(std::string i_ImagePath)
{
	bool result = m_Image.load(i_ImagePath);

	const auto numberOfPixelsX = m_Image.getWidth();
	const auto numberOfPixelsY = m_Image.getHeight();

	m_GridWidth = numberOfPixelsX / PixelsPerTile;
	m_GridHeight = numberOfPixelsY / PixelsPerTile;

	//create tiles according to the image
	auto& pixels = m_Image.getPixels();

	//check every tile
	for (int y = 0; y < m_GridHeight; ++y)
	{
		for (int x = 0; x < m_GridWidth; ++x)
		{
			int tileIndex = m_GridWidth * y + x;
			int obstaclePixelCount = 0;
			//check all pixels in the tile
			for (int i = 0; i < PixelsPerTile; ++i)
			{
				for (int j = 0; j < PixelsPerTile; ++j)
				{
					auto pixelX = x * PixelsPerTile + j;
					auto pixelY = y * PixelsPerTile + i;
					auto color = pixels.getColor(pixelX, pixelY);
					if (color == ofColor::black) obstaclePixelCount++;
				}
			}
			Node* tile = new Node(tileIndex, ofVec2f(x * PixelsPerTile, y* PixelsPerTile), obstaclePixelCount > pow(PixelsPerTile, 2) / 2);
			m_GraphNodes.push_back(tile);
		}
	}

}

Node* Grid::GetNodeByPosition(ofVec2f i_Position)
{
	auto gridX = i_Position.x / PixelsPerTile;
	auto gridY = i_Position.y / PixelsPerTile;

	return m_GraphNodes[(gridY * (m_GridWidth * gridX))];
}

std::vector<int> Grid::GetNeighboringTileIndices(int i_Index) const
{
	std::vector<int> returnVector;

	auto gridX = i_Index % m_GridWidth;
	auto gridY = i_Index / m_GridHeight;

	if (gridX > 0)
	{
		returnVector.push_back(i_Index - 1);
	}
	if (gridX < m_GridWidth - 1)
	{
		returnVector.push_back(i_Index + 1);
	}
	if (gridY > 0)
	{
		returnVector.push_back(i_Index - m_GridWidth);
	}
	if (gridY < m_GridHeight - 1)
	{
		returnVector.push_back(i_Index + m_GridWidth);
	}
	return returnVector;
}

std::vector<Node*> Grid::GetNeighboringNodesOfNode(int i_Index) const
{
	std::vector<Node*> returnVector;

	auto gridX = i_Index % m_GridWidth;
	auto gridY = i_Index / m_GridHeight;
	if (gridX > 0)
	{
		returnVector.push_back(GetNodeAtIndex(i_Index - 1));
	}
	if (gridX < m_GridWidth - 1)
	{
		returnVector.push_back(GetNodeAtIndex(i_Index + 1));
	}
	if (gridY > 0)
	{
		returnVector.push_back(GetNodeAtIndex(i_Index - m_GridWidth));
	}
	if (gridY < m_GridHeight - 1)
	{
		returnVector.push_back(GetNodeAtIndex(i_Index + m_GridWidth));
	}
	return returnVector;
}

std::vector<Node*> Grid::GetNeighboringNodesOfNode(const Node*& i_Node) const
{
	return GetNeighboringNodesOfNode(i_Node->m_Index);
}

void Grid::Draw()
{
	if (!m_bShouldVisualizeGraph) return;
	for (auto node : m_GraphNodes)
	{
		if (node->b_IsWalkable)
		{
			ofSetColor(0, 0, 255);
		}
		else
		{
			ofSetColor(255, 0, 0);
		}
		ofDrawRectangle(node->m_Position, PixelsPerTile, PixelsPerTile);
	}
}