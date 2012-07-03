//
// Copyright 2012 Josh Blum
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/test/unit_test.hpp>
#include <Theron/Framework.h>
#include <Theron/Actor.h>
#include <iostream>

//! Not trying to test theron itself, but rather verifying some intricacies

struct TestMessage0
{
    int number;
};

// A trivial actor that does nothing.
class TestActor0 : public Theron::Actor
{
public:

    TestActor0(void)
    {
        RegisterHandler(this, &TestActor0::handler); 
    }

    void handler(const TestMessage0 &message, const Theron::Address from)
    {
        this->number = message.number;
    }

    int number;
}; 

BOOST_AUTO_TEST_CASE(theron_test0)
{
    Theron::Framework framework;
    Theron::ActorRef actor0(framework.CreateActor<TestActor0>());
    Theron::ActorRef actor1(framework.CreateActor<TestActor0>());

    // Create a Receiver, which lets us receive messages sent by actors.
    Theron::Receiver receiver;

    TestMessage0 msg;
    msg.number = 42;
    actor0.Push(msg, receiver.GetAddress());
    msg.number = 12345;
    actor1.Push(msg, receiver.GetAddress());


}
