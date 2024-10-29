#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
	float position[3];
	float color[3];
	float texture[2];
	float textureType;
	float normal[3];
};

class Row;
class Column;

class Column
{
public:
	Column();
	float ratio;
	int rowCount;
	std::vector<Row> rows;
};

class Row
{
public:
	Row();
	float ratio;
	bool recursive;

	int texture;
	int type;
	int handlePosition;
	int handleOrientation;
	int handleTexture;
	float handleHorizontalOffset;
	float handleVerticalOffset;
	int columnCount;

	std::vector<Column> columns;
};

class Model
{
public:
	bool simplified;
	float height;
	float width;
	float depth;
	int texture;
	int columnCount;
	bool lighting;

	unsigned int offset = 0;
	float thickness = 0.02f;

	std::vector<Column> columns;

	enum class Normal
	{
		LEFT,
		RIGHT,
		FRONT,
		BACK,
		TOP,
		BOTTOM
	};

	void generateModel(glm::mat4& center, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void generateSquareSide(float constant, float y1, float z1, float y2, float z2, Normal norm, int tex, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void generateSquareFront(float constant, float x1, float y1, float x2, float y2, Normal norm, int tex, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void generateSquareFlat(float constant, float x1, float z1, float x2, float z2, Normal norm, int tex, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void generateNormal(Vertex& ver, Normal norm);
	void generateColumnsModel(float x1, float y1, float x2, float y2, std::vector<Column>& columns, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void generateRowsModel(float x1, float y1, float x2, float y2, std::vector<Row>& rows, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void generateDoorModel(float x1, float y1, float x2, float y2, Row& row, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void generateHandleModel(float x1, float y1, float x2, float y2, Row& row, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	bool generateGUI();
	void generateColumnsGui(int& columnCount, std::vector<Column>& columns, std::string str);
	void generateRowsGui(int& rowCount, std::vector<Row>& columns, std::string str);

	Model(GLFWwindow* window);
	~Model();	
};