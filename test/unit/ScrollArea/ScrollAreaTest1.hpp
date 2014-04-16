#ifndef _SCROLLAREATEST1_HPP_
#define _SCROLLAREATEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ScrollAreaTest1: public testing::Test
{
public:
	ScrollAreaTest1 ();
	virtual ~ScrollAreaTest1 ();

protected:

	virtual void SetUp ()
	{
#ifdef DEBUG
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
#endif
	}

	virtual void TearDown ()
	{
#ifdef DEBUG
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
#endif
	}
};

#endif  // _SCROLLAREATEST1_HPP_