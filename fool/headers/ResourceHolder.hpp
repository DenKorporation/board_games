#pragma once

#include "ResourceIdentifiers.h"

#include <memory>
#include <map>
#include <string>
#include <stdexcept>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void load(Identifier id, const std::string &filename);
	template <typename Parameter>
	void load(Identifier id, const std::string &filename, const Parameter &secondParameter);

	Resource &get(Identifier id);
	const Resource &get(Identifier id) const;

private:
	void insertResource(Identifier id, std::unique_ptr<Resource>);

private:
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include <ResourceHolder.inl>