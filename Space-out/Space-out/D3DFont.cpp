#include "D3DFont.h"

#include "..\DirectXTK\Inc\DDSTextureLoader.h"

D3DFont::D3DFont()
{
	m_Font = 0;
	m_srv = 0;
}

D3DFont::~D3DFont(){}

bool D3DFont::Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext, char* fontFilename, std::wstring* textureFilename)
{
	bool result;
	
	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if(!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, deviceContext, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

void D3DFont::Shutdown()
{
	// Release the font data.
	ReleaseFontData();
	return;
}

bool D3DFont::LoadFontData(char* filename)
{
	std::ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	m_Font = new FontType[95];
	if(!m_Font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for(i=0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

void D3DFont::ReleaseFontData()
{
	// Release the font data array.
	if(m_Font)
	{
		delete [] m_Font;
		m_Font = 0;
	}

	return;
}

bool D3DFont::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::wstring* filename)
{		
	HRESULT yr = DirectX::CreateDDSTextureFromFile(device, filename->c_str(), nullptr,  &m_srv );
	if(yr == S_OK)
		return true;
	else
		return false;
}

ID3D11ShaderResourceView* D3DFont::GetTexture()
{
	return m_srv;
}


void D3DFont::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	// Draw each letter onto a quad.
	for(i=0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if(letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			//// First triangle in quad.
			//vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			//vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			//index++;

			//vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			//vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			//index++;

			//vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			//vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			//index++;

			//// Second triangle in quad.
			//vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			//vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			//index++;

			//vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			//vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			//index++;

			//vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			//vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			//index++;

			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			FontType temp = m_Font[letter];

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}

	return;
}
