#include <libxml/parser.h>
#include <memory.h>
#include <iostream>

#include "GMailXmlParser.h"

void GMailXmlParser::initialize()
{
	LIBXML_TEST_VERSION
}

void GMailXmlParser::deinitialize()
{
}

bool GMailXmlParser::parse(const std::string& buffer)
{
	xmlSAXHandler handler;
	memset(&handler, 0, sizeof(handler) );
	handler.initialized = XML_SAX2_MAGIC;
	handler.startElementNs = &GMailXmlParser::startElementNs;
	handler.endElementNs = &GMailXmlParser::endElementNs;
	handler.characters = &GMailXmlParser::characters;
	handler.warning = &GMailXmlParser::warning;
	handler.error = &GMailXmlParser::error;
	state_ = State_Initial;
	items_.clear();
	int result = xmlSAXUserParseMemory(&handler, NULL, buffer.c_str(), buffer.size());
	return ( result == 0 ? true : false );
}

const std::vector<Message> GMailXmlParser::getItems()
{
	 return items_;
}


void GMailXmlParser::startElementNs( void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI, int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted, const xmlChar ** attributes )
{
	if (!strcmp((const char*) localname,"entry")) {
		GMailXmlParser::current_item_ = Message();
		GMailXmlParser::state_ = State_InEntry;
	}
	else if (!strcmp((const char*) localname,"title")) {
		GMailXmlParser::state_ = State_InTitle;
	}
	else if (!strcmp((const char*) localname,"summary")) {
		GMailXmlParser::state_ = State_InSummary;
	}
	else if (!strcmp((const char*) localname,"author")) {
		GMailXmlParser::state_ = State_InAuthor;
	}
	else if (!strcmp((const char*) localname,"name")) {
		GMailXmlParser::state_ = State_InAuthorName;
	}
	else if (!strcmp((const char*) localname,"email")) {
		GMailXmlParser::state_ = State_InAuthorEmail;
	}
}

void GMailXmlParser::endElementNs( void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI )
{
	if (!strcmp((const char*) localname,"entry")) {
		GMailXmlParser::items_.push_back(GMailXmlParser::current_item_);
	}
	else if (!strcmp((const char*) localname,"title")) {
		GMailXmlParser::state_ = State_InEntry;
	}
	else if (!strcmp((const char*) localname,"summary")) {
		GMailXmlParser::state_ = State_InEntry;
	}
	else if (!strcmp((const char*) localname,"author")) {
		GMailXmlParser::state_ = State_InEntry;
	}
	else if (!strcmp((const char*) localname,"name")) {
		GMailXmlParser::state_ = State_InAuthor;
	}
	else if (!strcmp((const char*) localname,"email")) {
		GMailXmlParser::state_ = State_InAuthor;
	}
}

void GMailXmlParser::characters( void *ctx, const xmlChar *value, int len)
{
	std::string data((const char*) value,len);
	switch (GMailXmlParser::state_) {
		case State_InTitle:
			GMailXmlParser::current_item_.title.append(data);
			break;
		case State_InSummary:
			GMailXmlParser::current_item_.summary.append(data);
			break;
		case State_InAuthorName:
			GMailXmlParser::current_item_.author_name.append(data);
			break;
		case State_InAuthorEmail:
			GMailXmlParser::current_item_.author_email.append(data);
			break;
		default:
			break;
	}
}

void GMailXmlParser::error( void * ctx, const char * msg, ... )
{
	std::cerr << "Xml parse error: " << msg << std::endl;
}

void GMailXmlParser::warning( void * ctx, const char * msg, ... )
{
	std::cerr << "Xml parse warning: " << msg << std::endl;
}

std::vector<Message> GMailXmlParser::items_;
Message GMailXmlParser::current_item_;
GMailXmlParser::State GMailXmlParser::state_;
