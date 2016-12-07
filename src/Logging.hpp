#pragma once
#include <sstream>
#include <glm/glm.hpp>
#include <ctime>

extern bool CLOG_SPECIAL_VALUE;

namespace pmk {
	inline void toStream(std::ostream& out){}
	template <typename T>
	inline void toStream(std::ostream& out, T val){
		out<<val<<" ";
	}
	template <typename T, typename... Args>
	inline void toStream(std::ostream& out, T val, Args... args){
		toStream(out, val);
		toStream(out, args...);
	}

	template <>
	inline void toStream(std::ostream& out, glm::vec2 val){
		out<<"[ "<<val.x<<", "<<val.y<<" ] ";
	}
	template <>
	inline void toStream(std::ostream& out, glm::ivec2 val){
		out<<"[ "<<val.x<<", "<<val.y<<" ] ";
	}
	template <>
	inline void toStream(std::ostream& out, glm::vec4 val){
		out<<"[ "<<val.x<<", "<<val.y<<", "<<val.z<<", "<<val.w<<" ] ";
	}
	template <>
	inline void toStream(std::ostream& out, glm::ivec4 val){
		out<<"[ "<<val.x<<", "<<val.y<<", "<<val.z<<", "<<val.w<<" ] ";
	}
	template <>
	inline void toStream(std::ostream& out, glm::vec3 val){
		out<<"[ "<<val.x<<", "<<val.y<<", "<<val.z<<" ] ";
	}
	template <>
	inline void toStream(std::ostream& out, glm::ivec3 val){
		out<<"[ "<<val.x<<", "<<val.y<<", "<<val.z<<" ] ";
	}
	template <typename T>
	inline void toStream(std::ostream& out, const std::vector<T> &val){
		out<<"{ ";
		for(auto &it : val){
			toStream(out, it);
		}
		out<<" } ";
	}

	extern std::vector<std::string> logBuffer;
	void openLogFile();
	void dumpLogBufferTofile();
}

template <typename... Args>
std::string toString(const Args &... args){
	std::stringstream ss;
	pmk::toStream(ss, args...);
	return ss.str();
}

template <typename... Args>
void toFile(const Args &... args){
	std::stringstream ss;
	pmk::toStream(ss, args...);
	pmk::logBuffer.push_back(toString(ss.str()));
}

template <typename... Args>
void log(const Args &... args){
	pmk::toStream(std::cout, args...);
	std::cout<<std::endl;
}

template <typename... Args>
void clog(const Args &... args){
	if(not CLOG_SPECIAL_VALUE) return;
	pmk::toStream(std::cout, args...);
	std::cout<<std::endl;
}

template <typename... Args>
void error(const Args &... args){
	std::cerr<<"[ ERROR ] ";
	pmk::toStream(std::cout, args...);
	std::cerr<<std::endl;
}

template <typename T, typename... Args>
void info(const T &t, const Args &... args){
	std::cout<<"[ "<<t<<" ] ";
	pmk::toStream(std::cout, args...);
	std::cout<<""<<std::endl;
}

#define LOG_LINE log(__FILE__, __LINE__);

inline void hardPause(){
	std::cin.ignore();
}
