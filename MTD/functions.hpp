#ifndef MTD_FUNCTIONS_HPP
#define MTD_FUNCTIONS_HPP

#include <string>
#include <Plinth/Math.hpp>
#include <SelbaWard/ConsoleScreen.hpp>
#include <Kairos/Timestep.hpp>

inline unsigned int getColumnToCenterString(const Cs& cs, const std::string& string)
{
	return (cs.getMode().x - static_cast<unsigned int>(string.size())) / 2u;
}

inline bool blink(kairos::Timestep& timestep, double hold, double duration)
{
	return pl::mod(timestep.getTime(), duration) < hold;
}

template <class T, class U>
void addElementToVectorIfUnique(std::vector<T>& vector, const U& elementToAdd)
{
	const T element{ static_cast<T>(elementToAdd) };
	if (std::find(vector.begin(), vector.end(), element) == vector.end())
		vector.emplace_back(element);
}

#endif // MTD_FUNCTIONS_HPP
