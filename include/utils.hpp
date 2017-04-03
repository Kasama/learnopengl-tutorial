#ifndef TUTORIAL_UTILS_HPP
#define TUTORIAL_UTILS_HPP
#pragma once

#include <defer.hpp>
#include <string>

#ifdef _MSC_VER
#define MSVC
#endif

#ifdef MSVC
#include <Windows.h>
#define RaiseBreakpoint() do {if(IsDebuggerPresent()) { DebugBreak(); }} while(0)
#else
#include <csignal>
#define RaiseBreakpoint() std::raise(SIGKILL)
#endif

#include <cstdio>
#define Assert(Exp) do {if(!(Exp)) { fprintf(stderr, "Assertion Failed: (%s) on %s:%d\n", #Exp, __FILE__, __LINE__); RaiseBreakpoint(); }} while(0)
#define StaticAssert(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

#define LogError(...) fprintf(stderr, __VA_ARGS__)

// GLM includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
//using namespace glm;
#define Pi glm::pi<float>()


#endif //TUTORIAL_UTILS_HPP
