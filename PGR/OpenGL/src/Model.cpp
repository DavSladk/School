#include <Model.h>

void Model::generateModel(glm::mat4& center, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	vertices.clear();
	indices.clear();
	offset = 0;

	center = glm::mat4(1.0f);
	center = glm::translate(center, glm::vec3(-(width / 2), -(height / 2), -(depth / 2)));

	Vertex tmpVer;

	// Left outter
	generateSquareSide(0.0f, 0.0f, 0.0f, height, depth, Normal::LEFT, texture, vertices, indices);
	// Left inner
	generateSquareSide(thickness, thickness, 0.0f, height - thickness, depth, Normal::RIGHT, texture, vertices, indices);
	// Right outter
	generateSquareSide(width, 0.0f, 0.0f, height, depth, Normal::RIGHT, texture, vertices, indices);
	// Right inner
	generateSquareSide(width - thickness, thickness, 0.0f, height - thickness, depth, Normal::LEFT, texture, vertices, indices);

	// Left front
	generateSquareFront(depth, 0.0f, 0.0f, thickness, height, Normal::FRONT, texture, vertices, indices);
	// Right front
	generateSquareFront(depth, width - thickness, 0.0f, width, height, Normal::FRONT, texture, vertices, indices);
	// Top front
	generateSquareFront(depth, thickness, height - thickness, width - thickness, height, Normal::FRONT, texture, vertices, indices);
	// Botton front
	generateSquareFront(depth, thickness, 0.0f, width - thickness, thickness, Normal::FRONT, texture, vertices, indices);
	// Back outer
	generateSquareFront(0.0f, 0.0f, 0.0f, width, height, Normal::BACK, texture, vertices, indices);
	// Back inner
	generateSquareFront(thickness, thickness, thickness, width-thickness, height-thickness, Normal::FRONT, texture, vertices, indices);

	// Top outter
	generateSquareFlat(height, 0.0f, 0.0f, width, depth, Normal::TOP, texture, vertices, indices);
	// Top inner
	generateSquareFlat(height - thickness, thickness, 0.0f, width - thickness, depth, Normal::BOTTOM, texture, vertices, indices);
	// Botton outter
	generateSquareFlat(0.0f, 0.0f, 0.0f, width, depth, Normal::BOTTOM, texture, vertices, indices);
	// Botton inner
	generateSquareFlat(thickness, thickness, 0.0f, width - thickness, depth, Normal::TOP, texture, vertices, indices);

	// Generate columns
	generateColumnsModel(thickness, thickness, width - thickness, height - thickness, columns, vertices, indices);
}

void Model::generateSquareSide(float constant, float y1, float z1, float y2, float z2, Normal norm, int tex, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	Vertex tmpVer;

	tmpVer.color[0] = 0.0f;
	tmpVer.color[1] = 0.0f;
	tmpVer.color[2] = 0.0f;
	tmpVer.position[0] = constant;
	tmpVer.textureType = tex;
	generateNormal(tmpVer, norm);

	tmpVer.position[1] = y1;
	tmpVer.position[2] = z1;
	tmpVer.texture[0] = 0.0f;
	tmpVer.texture[1] = 0.0f;
	vertices.push_back(tmpVer);

	tmpVer.position[1] = y1;
	tmpVer.position[2] = z2;
	tmpVer.texture[0] = 1.0f;
	tmpVer.texture[1] = 0.0f;
	vertices.push_back(tmpVer);

	tmpVer.position[1] = y2;
	tmpVer.position[2] = z2;
	tmpVer.texture[0] = 1.0f;
	tmpVer.texture[1] = 1.0f;
	vertices.push_back(tmpVer);

	tmpVer.position[1] = y2;
	tmpVer.position[2] = z1;
	tmpVer.texture[0] = 0.0f;
	tmpVer.texture[1] = 1.0f;
	vertices.push_back(tmpVer);

	indices.push_back(offset * 4 + 0);
	indices.push_back(offset * 4 + 1);
	indices.push_back(offset * 4 + 2);
	indices.push_back(offset * 4 + 2);
	indices.push_back(offset * 4 + 3);
	indices.push_back(offset * 4 + 0);
	offset++;
}

void Model::generateSquareFront(float constant, float x1, float y1, float x2, float y2, Normal norm, int tex, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	Vertex tmpVer;

	tmpVer.color[0] = 0.0f;
	tmpVer.color[1] = 0.0f;
	tmpVer.color[2] = 0.0f;
	tmpVer.position[2] = constant;
	tmpVer.textureType = tex;
	generateNormal(tmpVer, norm);

	tmpVer.position[0] = x1;
	tmpVer.position[1] = y1;
	tmpVer.texture[0] = 0.0f;
	tmpVer.texture[1] = 0.0f;
	vertices.push_back(tmpVer);

	tmpVer.position[0] = x2;
	tmpVer.position[1] = y1;
	tmpVer.texture[0] = 1.0f;
	tmpVer.texture[1] = 0.0f;
	vertices.push_back(tmpVer);

	tmpVer.position[0] = x2;
	tmpVer.position[1] = y2;
	tmpVer.texture[0] = 1.0f;
	tmpVer.texture[1] = 1.0f;
	vertices.push_back(tmpVer);

	tmpVer.position[0] = x1;
	tmpVer.position[1] = y2;
	tmpVer.texture[0] = 0.0f;
	tmpVer.texture[1] = 1.0f;
	vertices.push_back(tmpVer);

	indices.push_back(offset * 4 + 0);
	indices.push_back(offset * 4 + 1);
	indices.push_back(offset * 4 + 2);
	indices.push_back(offset * 4 + 2);
	indices.push_back(offset * 4 + 3);
	indices.push_back(offset * 4 + 0);
	offset++;
}

void Model::generateSquareFlat(float constant, float x1, float z1, float x2, float z2, Normal norm, int tex, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	Vertex tmpVer;

	tmpVer.color[0] = 0.0f;
	tmpVer.color[1] = 0.0f;
	tmpVer.color[2] = 0.0f;
	tmpVer.position[1] = constant;
	tmpVer.textureType = tex;
	generateNormal(tmpVer, norm);

	tmpVer.position[0] = x1;
	tmpVer.position[2] = z1;
	tmpVer.texture[0] = 0.0f;
	tmpVer.texture[1] = 0.0f;
	vertices.push_back(tmpVer);

	tmpVer.position[0] = x1;
	tmpVer.position[2] = z2;
	tmpVer.texture[0] = 1.0f;
	tmpVer.texture[1] = 0.0f;
	vertices.push_back(tmpVer);

	tmpVer.position[0] = x2;
	tmpVer.position[2] = z2;
	tmpVer.texture[0] = 1.0f;
	tmpVer.texture[1] = 1.0f;
	vertices.push_back(tmpVer);

	tmpVer.position[0] = x2;
	tmpVer.position[2] = z1;
	tmpVer.texture[0] = 0.0f;
	tmpVer.texture[1] = 1.0f;
	vertices.push_back(tmpVer);

	indices.push_back(offset * 4 + 0);
	indices.push_back(offset * 4 + 1);
	indices.push_back(offset * 4 + 2);
	indices.push_back(offset * 4 + 2);
	indices.push_back(offset * 4 + 3);
	indices.push_back(offset * 4 + 0);
	offset++;
}

void Model::generateNormal(Vertex& ver, Normal norm)
{
	switch (norm)
	{
	case Model::Normal::LEFT:
		ver.normal[0] = -1.0f;
		ver.normal[1] = 0.0f;
		ver.normal[2] = 0.0f;
		break;
	case Model::Normal::RIGHT:
		ver.normal[0] = 1.0f;
		ver.normal[1] = 0.0f;
		ver.normal[2] = 0.0f;
		break;
	case Model::Normal::FRONT:
		ver.normal[0] = 0.0f;
		ver.normal[1] = 0.0f;
		ver.normal[2] = 1.0f;
		break;
	case Model::Normal::BACK:
		ver.normal[0] = 0.0f;
		ver.normal[1] = 0.0f;
		ver.normal[2] = -1.0f;
		break;
	case Model::Normal::TOP:
		ver.normal[0] = 0.0f;
		ver.normal[1] = 1.0f;
		ver.normal[2] = 0.0f;
		break;
	case Model::Normal::BOTTOM:
		ver.normal[0] = 0.0f;
		ver.normal[1] = -1.0f;
		ver.normal[2] = 0.0f;
		break;
	default:
		break;
	}
}

void Model::generateColumnsModel(float x1, float y1, float x2, float y2, std::vector<Column>& columns, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	// Exit if invalid amount of columns
	if (columns.size() < 1)
	{
		return;
	}
	// Generate separators, when theyt make sense
	else if (columns.size() > 1)
	{
		// counter for ration
		float parts = 0;
		// counter for offseting
		float partsOffset = 0;
		// size of single part
		float partSize = 0;

		// count up all rations
		for (auto& column : columns)
		{
			parts += column.ratio;
		}

		// calculate size of single size
		partSize = (x2 - x1) / parts;

		// generate separators
		for (int i = 0; i < columns.size(); i++)
		{
			// except for the last column
			if (i < columns.size() - 1)
			{
				// Separator left side
				generateSquareSide(x1 + columns[i].ratio * partSize - thickness / 2.0f + partsOffset * partSize, y1, 0.0f, y2, depth, Normal::LEFT, texture, vertices, indices);
				// Separator right side
				generateSquareSide(x1 + columns[i].ratio * partSize + thickness / 2.0f + partsOffset * partSize, y1, 0.0f, y2, depth, Normal::RIGHT, texture, vertices, indices);
				// Separator front
				generateSquareFront(depth, x1 + columns[i].ratio * partSize - thickness / 2.0f + partsOffset * partSize, y1, x1 + columns[i].ratio * partSize + thickness / 2.0f + partsOffset * partSize, y2, Normal::FRONT, texture, vertices, indices);
			}

			// Generate rows in column
			if (i == 0)
			{
				generateRowsModel(x1, y1, x1 + columns[i].ratio * partSize - thickness / 2.0f, y2, columns[i].rows, vertices, indices);
			}
			else if (i == columns.size() - 1)
			{
				generateRowsModel(x1 + partsOffset * partSize + thickness / 2.0f, y1, x2, y2, columns[i].rows, vertices, indices);
			}
			else
			{
				generateRowsModel(x1 + partsOffset * partSize + thickness / 2.0f, y1, x1 + columns[i].ratio * partSize + partsOffset * partSize - thickness / 2.0f, y2, columns[i].rows, vertices, indices);
			}

			partsOffset += columns[i].ratio;
		}
	}
	// If only one column, do not render any separators.
	else // size == 1
	{
		generateRowsModel(x1, y1, x2, y2, columns[0].rows, vertices, indices);
	}
}

void Model::generateRowsModel(float x1, float y1, float x2, float y2, std::vector<Row>& rows, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	if (rows.size() < 1)
	{
		return;
	}
	else if (rows.size() > 1)
	{
		float parts = 0;
		float partsOffset = 0;
		float partSize = 0;

		for (auto& row : rows)
		{
			parts += row.ratio;
		}

		partSize = (y2 - y1) / (float)parts;

		for (int i = 0; i < rows.size(); i++)
		{
			if (i < rows.size() - 1)
			{
				// Separator upper side
				generateSquareFlat(y1 + rows[i].ratio * partSize + thickness / 2.0f + partsOffset * partSize, x1, 0.0f, x2, depth, Normal::TOP, texture, vertices, indices);
				// Separator lower side
				generateSquareFlat(y1 + rows[i].ratio * partSize - thickness / 2.0f + partsOffset * partSize, x1, 0.0f, x2, depth, Normal::BOTTOM, texture, vertices, indices);
				// Separator front 
				generateSquareFront(depth, x1, y1 + rows[i].ratio * partSize - thickness / 2.0f + partsOffset * partSize, x2, y1 + rows[i].ratio * partSize + thickness / 2 + partsOffset * partSize, Normal::FRONT, texture, vertices, indices);
			}

			if (rows[i].recursive)
			{

				if (i == 0)
				{
					generateColumnsModel(x1, y1, x2, y1 + rows[i].ratio * partSize - thickness / 2.0f, rows[i].columns, vertices, indices);
				}
				else if (i == rows.size() - 1)
				{
					generateColumnsModel(x1, y1 + thickness / 2.0f + partsOffset * partSize, x2, y2, rows[i].columns, vertices, indices);
				}
				else
				{
					generateColumnsModel(x1, y1 + thickness / 2.0f + partsOffset * partSize, x2, y1 + rows[i].ratio * partSize - thickness / 2.0f + partsOffset * partSize, rows[i].columns, vertices, indices);
				}
			}
			else
			{
				if (rows[i].type == 0)
				{
					// generate nothing
				}
				else if (rows[i].type == 1)
				{
					if (i == 0)
					{
						generateDoorModel(x1, y1, x2, y1 + rows[i].ratio * partSize - thickness / 2.0f, rows[i], vertices, indices);
						generateHandleModel(x1, y1, x2, y1 + rows[i].ratio * partSize - thickness / 2.0f, rows[i], vertices, indices);
					}
					else if (i == rows.size() - 1)
					{
						generateDoorModel(x1, y1 + thickness / 2.0f + partsOffset * partSize, x2, y2, rows[i], vertices, indices);
						generateHandleModel(x1, y1 + thickness / 2.0f + partsOffset * partSize, x2, y2, rows[i], vertices, indices);
					}
					else
					{
						generateDoorModel(x1, y1 + thickness / 2.0f + partsOffset * partSize, x2, y1 + rows[i].ratio * partSize - thickness / 2.0f + partsOffset * partSize, rows[i], vertices, indices);
						generateHandleModel(x1, y1 + thickness / 2.0f + partsOffset * partSize, x2, y1 + rows[i].ratio * partSize - thickness / 2.0f + partsOffset * partSize, rows[i], vertices, indices);
					}
				}
			}

			partsOffset += rows[i].ratio;
		}
	}
	else // size == 1
	{
		if (rows[0].recursive)
		{
			generateColumnsModel(x1, y1, x2, y2, rows[0].columns, vertices, indices);
		}
		else
		{
			if (rows[0].type == 0)
			{
				// generate nothing
			}
			else if (rows[0].type == 1)
			{
				generateDoorModel(x1, y1, x2, y2, rows[0], vertices, indices);
				generateHandleModel(x1, y1, x2, y2, rows[0], vertices, indices);
			}
		}
	}
}

void Model::generateDoorModel(float x1, float y1, float x2, float y2, Row& row, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	// Generate top
	generateSquareFlat(y2 + thickness / 3.0f, x1 - thickness / 3.0f, depth, x2 + thickness / 3.0f, depth + thickness, Normal::TOP, row.texture, vertices, indices);
	// Generate left
	generateSquareSide(x1 - thickness / 3.0f, y1 - thickness / 3.0f, depth, y2 + thickness / 3.0f, depth + thickness, Normal::LEFT, row.texture, vertices, indices);
	// Generate right
	generateSquareSide(x2 + thickness / 3.0f, y1 - thickness / 3.0f, depth, y2 + thickness / 3.0f, depth + thickness, Normal::RIGHT, row.texture, vertices, indices);
	// Generate bottom
	generateSquareFlat(y1 - thickness / 3.0f, x1 - thickness / 3.0f, depth, x2 + thickness / 3.0f, depth + thickness, Normal::BOTTOM, row.texture, vertices, indices);
	// Generate front
	generateSquareFront(depth + thickness, x1 - thickness / 3.0f, y1 - thickness / 3.0f, x2 + thickness / 3.0f, y2 + thickness / 3.0f, Normal::FRONT, row.texture, vertices, indices);
}

void Model::generateHandleModel(float x1, float y1, float x2, float y2, Row& row, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	float handleOffsetHorizontal = 0.0f;
	float handleOffsetVertical = 0.0f;
	int texture = row.handleTexture;

	if (simplified)
	{
		texture = 0;
		if (row.handlePosition == 1)
		{
			handleOffsetHorizontal = x1 + (x2 - x1) / 2;
			handleOffsetVertical = y2 - 2 * thickness;
		}
		else if (row.handlePosition == 2)
		{
			handleOffsetHorizontal = x2 - 2 * thickness;
			handleOffsetVertical = y1 + (y2 - y1) / 2;
		}
		else if (row.handlePosition == 3)
		{
			handleOffsetHorizontal = x1 + (x2 - x1) / 2;
			handleOffsetVertical = y1 + 2 * thickness;
		}
		else if (row.handlePosition == 4)
		{
			handleOffsetHorizontal = x1 + 2 * thickness;
			handleOffsetVertical = y1 + (y2 - y1) / 2;
		}
		else
		{
			handleOffsetHorizontal = x1 + (x2 - x1) / 2;
			handleOffsetVertical = y1 + (y2 - y1) / 2;
		}
	}
	else
	{
		handleOffsetHorizontal = x1 + (x2 - x1) / 2 + row.handleHorizontalOffset;
		handleOffsetVertical = y1 + (y2 - y1) / 2 + row.handleVerticalOffset;
	}
	

	if (row.handleOrientation == 0)
	{
		// Left outter
		generateSquareSide(-3 * thickness + handleOffsetHorizontal, -thickness / 2 + handleOffsetVertical, depth + thickness, thickness / 2 + handleOffsetVertical, depth + 3 * thickness, Normal::LEFT, texture, vertices, indices);
		// Left inner
		generateSquareSide(-2 * thickness + handleOffsetHorizontal, -thickness / 2 + handleOffsetVertical, depth + thickness, thickness / 2 + handleOffsetVertical, depth + 2 * thickness, Normal::RIGHT, texture, vertices, indices);
		// Left upper
		generateSquareFlat(thickness / 2 + handleOffsetVertical, -3 * thickness + handleOffsetHorizontal, depth + thickness, -2 * thickness + handleOffsetHorizontal, depth + 2 * thickness, Normal::TOP, texture, vertices, indices);
		// Left bottom
		generateSquareFlat(-thickness / 2 + handleOffsetVertical, -3 * thickness + handleOffsetHorizontal, depth + thickness, -2 * thickness + handleOffsetHorizontal, depth + 2 * thickness, Normal::BOTTOM, texture, vertices, indices);
		// Right outter
		generateSquareSide(3 * thickness + handleOffsetHorizontal, -thickness / 2 + handleOffsetVertical, depth + thickness, thickness / 2 + handleOffsetVertical, depth + 3 * thickness, Normal::RIGHT, texture, vertices, indices);
		// Right inner
		generateSquareSide(2 * thickness + handleOffsetHorizontal, -thickness / 2 + handleOffsetVertical, depth + thickness, thickness / 2 + handleOffsetVertical, depth + 2 * thickness, Normal::LEFT, texture, vertices, indices);
		// Right upper
		generateSquareFlat(thickness / 2 + handleOffsetVertical, 3 * thickness + handleOffsetHorizontal, depth + thickness, 2 * thickness + handleOffsetHorizontal, depth + 2 * thickness, Normal::TOP, texture, vertices, indices);
		// Right bottom
		generateSquareFlat(-thickness / 2 + handleOffsetVertical, 3 * thickness + handleOffsetHorizontal, depth + thickness, 2 * thickness + handleOffsetHorizontal, depth + 2 * thickness, Normal::BOTTOM, texture, vertices, indices);
		// Front outter
		generateSquareFront(depth + thickness * 3, -3 * thickness + handleOffsetHorizontal, -thickness / 2 + handleOffsetVertical, 3 * thickness + handleOffsetHorizontal, thickness / 2 + handleOffsetVertical, Normal::FRONT, texture, vertices, indices);
		// Front inner
		generateSquareFront(depth + thickness * 2, -2 * thickness + handleOffsetHorizontal, -thickness / 2 + handleOffsetVertical, 2 * thickness + handleOffsetHorizontal, thickness / 2 + handleOffsetVertical, Normal::BACK, texture, vertices, indices);
		// Front upper
		generateSquareFlat(thickness / 2 + handleOffsetVertical, -3 * thickness + handleOffsetHorizontal, depth + 2 * thickness, 3 * thickness + handleOffsetHorizontal, depth + 3 * thickness, Normal::TOP, texture, vertices, indices);
		// Front bottom
		generateSquareFlat(-thickness / 2 + handleOffsetVertical, -3 * thickness + handleOffsetHorizontal, depth + 2 * thickness, 3 * thickness + handleOffsetHorizontal, depth + 3 * thickness, Normal::BOTTOM, texture, vertices, indices);
	}
	else if (row.handleOrientation == 1)
	{
		// Up upper
		generateSquareFlat(3 * thickness + handleOffsetVertical, -thickness / 2 + handleOffsetHorizontal, depth + thickness, thickness / 2 + handleOffsetHorizontal, depth + 3 * thickness, Normal::TOP, texture, vertices, indices);
		// Up bottom
		generateSquareFlat(2 * thickness + handleOffsetVertical, -thickness / 2 + handleOffsetHorizontal, depth + thickness, thickness / 2 + handleOffsetHorizontal, depth + 2 * thickness, Normal::BOTTOM, texture, vertices, indices);
		// Up left
		generateSquareSide(-thickness / 2 + handleOffsetHorizontal,2*thickness + handleOffsetVertical, depth + thickness, 3*thickness + handleOffsetVertical, depth + 3*thickness, Normal::LEFT, texture, vertices, indices );
		// Up right
		generateSquareSide(thickness / 2 + handleOffsetHorizontal, 2 * thickness + handleOffsetVertical, depth + thickness, 3 * thickness + handleOffsetVertical, depth + 3 * thickness, Normal::RIGHT, texture, vertices, indices);
		// Down upper
		generateSquareFlat(-2 * thickness + handleOffsetVertical, -thickness / 2 + handleOffsetHorizontal, depth + thickness, thickness / 2 + handleOffsetHorizontal, depth + 2 * thickness, Normal::TOP, texture, vertices, indices);
		// Down bottom
		generateSquareFlat(-3 * thickness + handleOffsetVertical, -thickness / 2 + handleOffsetHorizontal, depth + thickness, thickness / 2 + handleOffsetHorizontal, depth + 3 * thickness, Normal::BOTTOM, texture, vertices, indices);
		// Down left
		generateSquareSide(-thickness / 2 + handleOffsetHorizontal, -2 * thickness + handleOffsetVertical, depth + thickness, -3 * thickness + handleOffsetVertical, depth + 3 * thickness, Normal::LEFT, texture, vertices, indices);
		// Down right
		generateSquareSide(thickness / 2 + handleOffsetHorizontal, -2 * thickness + handleOffsetVertical, depth + thickness, -3 * thickness + handleOffsetVertical, depth + 3 * thickness, Normal::RIGHT, texture, vertices, indices);
		// Middle outter
		generateSquareFront(depth + 3 * thickness, -thickness / 2 + handleOffsetHorizontal, -3 * thickness + handleOffsetVertical, thickness / 2 + handleOffsetHorizontal, 3 * thickness + handleOffsetVertical, Normal::FRONT, texture, vertices, indices);
		// Middle inner
		generateSquareFront(depth + 2 * thickness, -thickness / 2 + handleOffsetHorizontal, -2 * thickness + handleOffsetVertical, thickness / 2 + handleOffsetHorizontal, 2 * thickness + handleOffsetVertical, Normal::BACK, texture, vertices, indices);
		// Middle left
		generateSquareSide(-thickness / 2 + handleOffsetHorizontal, -3 * thickness + handleOffsetVertical, depth + 2 *thickness, 3 * thickness + handleOffsetVertical, depth + 3 * thickness, Normal::LEFT, texture, vertices, indices);
		// Middle right
		generateSquareSide(thickness / 2 + handleOffsetHorizontal, -3 * thickness + handleOffsetVertical, depth + 2 * thickness, 3 * thickness + handleOffsetVertical, depth + 3 * thickness, Normal::RIGHT, texture, vertices, indices);
	}
}

bool Model::generateGUI()
{
	bool clicked = false;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;
	bool* p_open = NULL;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::Begin("Parametrs", p_open, window_flags);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::MenuItem("Simplified", NULL, &simplified);
				ImGui::MenuItem("Lighting", NULL, &lighting);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}	
		ImGui::InputFloat("Height", &height, 0.1f);
		ImGui::InputFloat("Width", &width, 0.1f);
		ImGui::InputFloat("Depth", &depth, 0.1f);
		ImGui::InputInt("Texture", &texture);
		ImGui::InputInt("Columns", &columnCount);

		if (ImGui::Button("Generate"))
		{
			clicked = true;
		}

		generateColumnsGui(columnCount, columns, "");

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return clicked;
}

void Model::generateColumnsGui(int& columnCount, std::vector<Column>& columns, std::string str)
{
	if (columnCount < 0)
	{
		return;
	}

	while (columnCount > columns.size())
	{
		columns.push_back(Column());
	}

	while (columnCount < columns.size())
	{
		columns.pop_back();
	}

	for (int i = 0; i < columns.size(); i++)
	{
		std::string number = str + std::to_string(i + 1) + ".";
		std::string text = number + " Column";

		if (ImGui::TreeNode(text.c_str()))
		{
			ImGui::InputFloat(("Ratio##" + number).c_str(), &(columns[i].ratio), 1.0f);
			ImGui::InputInt(("Rows##" + number).c_str(), &(columns[i].rowCount));
			generateRowsGui(columns[i].rowCount, columns[i].rows, number);
			ImGui::TreePop();
		}
	}
}

void Model::generateRowsGui(int& rowCount, std::vector<Row>& rows, std::string str)
{
	if (rowCount < 0)
	{
		return;
	}

	while (rowCount > rows.size())
	{
		rows.push_back(Row());
	}

	while (rowCount < rows.size())
	{
		rows.pop_back();
	}

	for (int i = 0; i < rows.size(); i++)
	{
		std::string number = str + std::to_string(i + 1) + ".";
		std::string text = number + " Row";

		if (ImGui::TreeNode(text.c_str()))
		{
			ImGui::InputFloat(("Ratio##" + number).c_str(), &(rows[i].ratio), 1.0f);
			ImGui::Checkbox(("Recursive##" + number).c_str(), &(rows[i].recursive));
			if (rows[i].recursive)
			{
				ImGui::InputInt(("Columns##" + number).c_str(), &(rows[i].columnCount));
				generateColumnsGui(rows[i].columnCount, rows[i].columns, number);
			}
			else
			{
				ImGui::InputInt("Texture", &(rows[i].texture));
				ImGui::InputInt("Type", &(rows[i].type));
				if (rows[i].type == 1)
				{
					if (simplified)
					{
						ImGui::InputInt("Handle Position", &(rows[i].handlePosition));
					}
					else
					{
						ImGui::InputFloat(("Horizontal##" + number).c_str(), &(rows[i].handleHorizontalOffset), 0.02f);
						ImGui::InputFloat(("Vertical##" + number).c_str(), &(rows[i].handleVerticalOffset), 0.02f);
						ImGui::InputInt("Handle Texture", &(rows[i].handleTexture));
					}
					ImGui::InputInt("Handle Orientation", &(rows[i].handleOrientation));					
				}
			}
			ImGui::TreePop();
		}
	}
}

Model::Model(GLFWwindow* window) :
	height(1), width(1), depth(1), texture(false), columnCount(1), simplified(true), lighting(false)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

Model::~Model()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

Column::Column() :
	ratio(1), rowCount(1)
{
}

Row::Row() :
	ratio(1), recursive(false), texture(0), columnCount(1), type(0), handlePosition(0), handleOrientation(0), handleTexture(0), handleHorizontalOffset(0), handleVerticalOffset(0)
{
}
