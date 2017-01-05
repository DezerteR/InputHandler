#pragma once
#include <sstream>
#include <glm/glm.hpp>
#include <ctime>
#include <type_traits>

//specialize a type for all of the STL containers.
namespace is_stl_container_impl{
  template <typename T>       struct is_stl_container:std::false_type{};
  // template <typename T, std::size_t N> struct is_stl_container<std::array    <T,N>>    :std::true_type{};
  template <typename... Args> struct is_stl_container<std::vector            <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::deque             <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::list              <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::forward_list      <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::set               <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::multiset          <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::map               <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::multimap          <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::unordered_set     <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::unordered_multiset<Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::unordered_map     <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::unordered_multimap<Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::stack             <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::queue             <Args...>>:std::true_type{};
  // template <typename... Args> struct is_stl_container<std::priority_queue    <Args...>>:std::true_type{};
}

//type trait to utilize the implementation type traits as well as decay the type
template <typename T> struct is_stl_container {
  static constexpr bool const value = is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};

extern bool CLOG_SPECIAL_VALUE;

namespace pmk {
	inline void toStream(std::ostream& out){}
	template <typename T, typename = std::enable_if_t<!is_stl_container<T>::value>>
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
	template <typename T, typename = std::enable_if_t<is_stl_container<T>::value>>
	inline void toStream(std::ostream& out, const std::vector<T> &val){
        std::cerr<<"asdf";
		out<<"{ ";
		// for(auto &it : val){
			// toStream(out, it);
		// }
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
