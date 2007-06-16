#ifndef GMAILXMLPARSER_H
#define GMAILXMLPARSER_H

// FIXME: Use exceptions, and return items directly

#include <libxml/parser.h>
#include <vector>

#include "Message.h"

class GMailXmlParser 
{
public:
	static void initialize();
	static void deinitialize();

	static bool parse(const std::string& buffer);
	static const std::vector<Message> getItems(); 

private:
	// LibXML2 interface
	static void startElementNs( void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI, int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted, const xmlChar ** attributes );
	static void endElementNs( void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI );
	static void characters( void *ctx, const xmlChar *value, int len);
	static void error( void * ctx, const char * msg, ... );
	static void warning( void * ctx, const char * msg, ... );

private:
	static std::vector<Message> items_;
	static Message current_item_;

	enum State {
		State_Initial,
		State_InEntry,
		State_InTitle,
		State_InSummary,
		State_InAuthor,
		State_InAuthorName,
		State_InAuthorEmail,
	};
	static State state_;
};

#endif
