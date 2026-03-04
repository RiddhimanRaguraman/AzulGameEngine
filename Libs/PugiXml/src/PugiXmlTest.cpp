//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "pugixml.hpp"

int main()
{
	Trace::out("PugiXml - test \n");


	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file("MetricsArial36.xml");
	assert(result);
	Trace::out("Load result: %s \n", result.description());
	Trace::out("  file name: %s \n", (doc.child("fontMetrics").attribute("file").value()));

	unsigned int key = 0;
	float x = -1.0;
	float y = -1.0;
	float width = -1.0;
	float height = -1.0;

	for(pugi::xml_node node = doc.child("fontMetrics").child("character"); node; node = node.next_sibling("character"))
	{
		key = node.attribute("key").as_uint();
		assert(key > 31);
		assert(key < 128);

		x = node.child("x").text().as_float();
		assert(x > -1);

		y = node.child("y").text().as_float();
		assert(y > -1);

		width = node.child("width").text().as_float();
		assert(width > -1);

		height = node.child("height").text().as_float();
		assert(height > -1);

		Trace::out("key: %d  (x,y,w,h) %f %f %f %f \n",
				   key, x, y, width, height);
	}

}

// ---  End of File ---------------
