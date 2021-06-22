// this transition class is for event performing
#pragma once
#include "process.hpp"
#include "scheduler.hpp"

using namespace std;
// class Transition :
// {
//     friend class StateMachine;
//     enum class TransitionType {
//         TT_NORMAL_TRANSITION,
//         TT_INTERNAL_TRANSITION,
//     };
// }


class Transition
{
    public:
        Transition();

        



};

//  public:


//  public:
//     static TransitionSharedPtr MakeTransition(const char* name,
//                 const StateSharedPtr& from,
//                 const StateSharedPtr& to,
//                 IPredicateSharedPtr pred);

//     static TransitionSharedPtr MakeTransition(const char* name,
//                 const StateSharedPtr& toFrom,
//                 IPredicateSharedPtr pred);