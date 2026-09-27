#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace RE { class TESWeather; }

class WeatherManager
{
public:
	static WeatherManager& GetSingleton();

	struct WeatherEntry
	{
		std::string fileName;
		std::vector<uint32_t> weatherIDs;
	};

	void Initialize();
	void LoadWeatherList();
	void LoadLocationWeather();
	void LoadWeatherRemaps();

	WeatherEntry* FindWeatherEntry(uint32_t weatherID);

	/// @brief Gets the effective weather ID, checking for location-based overrides first.
	/// @param actualWeatherID The real weather form ID from the game
	/// @return Location-mapped weather ID if applicable, otherwise the actual weather ID
	uint32_t GetEffectiveWeatherID(uint32_t actualWeatherID);
	uint32_t GetEffectiveWeatherID(RE::TESWeather* weather);

	const std::unordered_map<std::string, WeatherEntry>& GetWeatherEntries() const { return weatherEntries; }

	std::unordered_map<std::string, std::string> GetWeatherFiles() const;

private:
	std::unordered_map<std::string, WeatherEntry> weatherEntries;
	std::unordered_map<uint32_t, std::string> weatherIDMap;

	// Location weather: worldSpaceID -> (locationID -> fakeWeatherID)
	std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>> locationWeatherMap;

	struct WeatherRemap
	{
		uint32_t targetID = 0;
		std::string nameIs;
		std::string nameContains;
		std::string classification;
	};
	std::vector<WeatherRemap> weatherRemaps;

	void ParseWeatherIDs(const std::string& weatherIDsStr, std::vector<uint32_t>& weatherIDs);
	uint32_t ParseHexID(const std::string& hexStr);
};
