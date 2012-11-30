// Ŭnicode please
#pragma once

class TiXmlNode;

void dump_to_stdout(const char* pFilename);
void dump_to_stdout( TiXmlNode* pParent, unsigned int indent = 0 );
