#ifndef MY_SHADER_H
#define MY_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class myShader{
public:
	//����ID
	unsigned int ID;
	//���캯��
	myShader(const GLchar* vertexPath, const GLchar* fragmentPath, const char* geometryPath = nullptr){
		// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
		string vertexCode;
		string fragmentCode;
		string geometryCode;
		ifstream vShaderFile;  //������ɫ���ļ�
		ifstream fShaderFile;  //Ƭ����ɫ���ļ�
		ifstream gShaderFile;  //������ɫ���ļ�
		// ��֤ifstream��������׳��쳣
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try{
			// ���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			// ��ȡ�ļ��Ļ������ݵ���������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// �ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();
			// ת����������string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			// if geometry shader path is present, also load a geometry shader
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (ifstream::failure e) {
			cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		}
		const char* vShaderCode = vertexCode.c_str();  //ΪɶҪת����const char*?
		const char* fShaderCode = fragmentCode.c_str();

		// 2. ������ɫ��
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		// ������ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);  //Ҫ��&�� ȡһ��const char*�ĵ�ַ���ܰ�..?
		glCompileShader(vertex);
		// ��ӡ�����������еĻ���
		checkCompileErrors(vertex, "VERTEX");
		//Ƭ����ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		//������ɫ��
		unsigned int geometry;
		if (geometryPath != nullptr)
		{
			const char * gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}
		// ��ɫ������
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		if (geometryPath != nullptr)
			glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// ɾ����ɫ��
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometryPath != nullptr)
			glDeleteShader(geometry);
	}

	//ʹ��/�������
	void use(){
		glUseProgram(ID);
	}
	//�˴�Ϊ�Լ����� 
	void stop() {
		glUseProgram(0);
	}
	//set�������ܹ���ѯһ��unform��λ��ֵ����������ֵ
	void setBool(const string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
private:
	// ���ڼ����ɫ������/���Ӵ����ʵ�ó�����
	void checkCompileErrors(unsigned int shader, string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
			}
		}
	}

};

#endif