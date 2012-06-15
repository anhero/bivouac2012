/* 
 * File:   Bridge.h
 * Author: samuel
 *
 * Created on 15 juin 2012, 05:01
 */

#ifndef BRIDGE_H
#define	BRIDGE_H

#include <RedBox.h>
using namespace RedBox;
namespace Bivouac2012 {

class Bridge : public RedBox::Sprite  {
public:
	Bridge(Vector2 pos, bool horizontal);
private:
	bool _horizontal;
};
}

#endif	/* BRIDGE_H */

