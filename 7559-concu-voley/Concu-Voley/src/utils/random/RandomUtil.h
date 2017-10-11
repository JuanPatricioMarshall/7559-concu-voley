/*
 * RandomUtil.h
 *
 *  Created on: Oct 16, 2016
 *      Author: gaston
 */

#ifndef UTILS_RANDOM_RANDOMUTIL_H_
#define UTILS_RANDOM_RANDOMUTIL_H_

namespace std {

class RandomUtil {

private:

	static bool seeded;

public:
	RandomUtil();

	static double randomCeroUno();

	static void seed();

	static int randomInt(int max);

	virtual ~RandomUtil();
};

} /* namespace std */

#endif /* UTILS_RANDOM_RANDOMUTIL_H_ */
