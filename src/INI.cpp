#include "INI.hpp"

INI::INI()
{}

INI::INI(const std::string& path)
{
	load(path);
}

INI::~INI()
{}

void INI::clear()
{
	mErrors.clear();
	mSections.clear();
}

bool INI::load(const std::string& path)
{
	clear();

	std::ifstream stream(path);

	if (!stream.is_open())
		return false;

	std::vector<std::string> data;
	std::string line;

	while (std::getline(stream, line))
		data.push_back(line);

	return parse(data);
}

bool INI::save(const std::string& path)
{
	std::ofstream stream(path);

	for (std::pair<std::string, std::unordered_map<std::string, std::string>> section : mSections)
	{
		if (!section.first.empty())
			stream << "[" << section.first << "]\n";

		for (std::pair<std::string, std::string> element : section.second)
			stream << element.first << "=" << element.second << "\n";

		stream << "\n";
	}

	stream << "\n";

	return true;
}

bool INI::parse(const char *data, std::size_t size)
{
	std::vector<std::string> lines;
	lines.push_back("");

	for (std::size_t i = 0; i < size; ++i)
	{
		char c = data[i];

		if (c == '\n' || c == '\r')
			lines.push_back("");
		else
			lines.at(lines.size() - 1).push_back(c);;
	}

	return parse(lines);
}

bool INI::parse(const std::vector<char> &data)
{
	std::vector<std::string> lines;
	lines.push_back("");

	for (char c : data)
	{
		if (c == '\n' || c == '\r')
			lines.push_back("");
		else
			lines.at(lines.size() - 1).push_back(c);;
	}

	return parse(lines);
}

bool INI::parse(const std::vector<std::string> &data)
{
	std::string section, name, value;

	// for (std::string line : data)
	for (std::size_t i = 0; i < data.size(); ++i)
	{
		std::string line = data.at(i);

		// Trim whitespace at the beginning until there are none left.
		while (!line.empty() && isspace(line.at(0)))
			line.erase(line.begin());

		if (line.empty())
			continue;

		// Discard comment lines.
		if (line.at(0) == '#' || line.at(0) == ';')
			continue;

		if (line.at(0) == '[')
		{
			if (line.at(line.size() - 1) == ']')
				section = line.substr(1, line.size() - 2);
			else
				mErrors += std::string("Line " + std::to_string(i + 1) + " could not parse section\n");
		}
		else
		{
			int state = 0;

			for (std::size_t j = 0; j < line.size(); ++j)
			{
				char c = line.at(j);

				if (state == 0)
				{
					if (c == '=')
					{
						state = 1;
						continue;
					}
					else
					{
						if ((c >= 'A' && c <= 'Z') ||
						    (c >= 'a' && c <= 'z') ||
						    (c >= '0' && c <= '9') ||
						    (c == '_'))
						{
							name.push_back(c);
						}
						else
						{
							name = "";
							mErrors += std::string("Line " + std::to_string(i + 1) + " contained an invalid character '" + c + "'\n");
							break;
						}
					}
				}
				else if (state == 1)
				{
					value.push_back(c);
				}
			}
		}

		if (!name.empty())
			setValue(section, name, value);

		name = "";
		value = "";
	}

	return true;
}

std::string INI::getErrors() const
{
	return mErrors;
}

std::string INI::getString(const std::string &section, const std::string &name, const std::string &def)
{
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::const_iterator s = mSections.find(section);

	if (s != mSections.end())
	{
		std::unordered_map<std::string, std::string>::const_iterator k = s->second.find(name);

		if (k != s->second.end())
			return k->second;
	}

	return def;
}

int INI::getInteger(const std::string &section, const std::string &name, int def)
{
	return std::strtol(getString(section, name, std::to_string(def)).c_str(), NULL, 10);
}

float INI::getFloat(const std::string &section, const std::string &name, float def)
{
	return std::atof(getString(section, name, std::to_string(def)).c_str());
}

bool INI::getBoolean(const std::string &section, const std::string &name, bool def)
{
	std::string value = def ? getString(section, name, "true") : getString(section, name, "false");

	if (value == "1" || value == "true" || value == "True" || value == "TRUE")
		return true;

	if (value == "0" || value == "false" || value == "False" || value == "FALSE")
		return false;

	return def;
}

void INI::setValue(const std::string &section, const std::string &name, const std::string &value)
{
	mSections[section][name] = value;
}

void INI::setValue(const std::string &section, const std::string &name, int value)
{
	mSections[section][name] = std::to_string(value);
}

void INI::setValue(const std::string &section, const std::string &name, float value)
{
	mSections[section][name] = std::to_string(value);
}

void INI::setValue(const std::string &section, const std::string &name, bool value)
{
	value ? mSections[section][name] = "true" : mSections[section][name] = "false";
}
