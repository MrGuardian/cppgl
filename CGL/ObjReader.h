#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "Vertex.h"
using std::string;
using std::vector;

namespace CGL {
	struct Face {
		Vertex vertices[3];
	};

	class ObjReader {
	public:
		ObjReader(const string &filename) {
			std::ifstream file;
			file.open(filename);
			string line;
			while (std::getline(file, line)) {
				auto words = split(line, ' ');
				if (words.size() > 0 && words[0] == "v") {
					Vertex vertex;
					vertex.x = std::atof(words[1].data());
					vertex.y = std::atof(words[2].data());
					vertex.z = std::atof(words[3].data());
					_vertices.push_back(vertex);
				}
				else if (words.size() > 0 && words[0] == "f") {
					Face face;
					auto faceCoords = split(words[1], '/');
					face.vertices[0] = _vertices[std::atoi(faceCoords[0].data()) - 1];
					faceCoords = split(words[2], '/');
					face.vertices[1] = _vertices[std::atoi(faceCoords[0].data()) - 1]; 
					faceCoords = split(words[3], '/');
					face.vertices[2] = _vertices[std::atoi(faceCoords[0].data()) - 1];
					_faces.push_back(face);
				}
			}
			file.close();
		}

		const vector<Vertex>& vertices() const { return _vertices; }
		const vector<Face>& faces() const { return _faces; }

	private:
		vector<Vertex> _vertices;
		vector<Face> _faces;

		vector<string> split(const string &s, const char delim) {
			vector<string> elems;
			split(s, delim, elems);
			return elems;
		}

		vector<std::string> &split(const std::string &s, const char delim, std::vector<std::string> &elems) {
			std::stringstream ss(s);
			std::string item;
			while (std::getline(ss, item, delim)) {
				elems.push_back(item);
			}
			return elems;
		}
	};
}