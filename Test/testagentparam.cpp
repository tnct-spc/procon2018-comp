#include "testagentparam.h"
#include "geneticalgo/geneticagent.h"

TestAgentParam::TestAgentParam()
{

}

void TestAgentParam::run(){

    //テストなので複数アルゴリズムへの対応はしません！やった！
    //両方のエージェントに対して同じアルゴリズムを使うよ

    random_agents.push_back(GeneticAgent(6));

}
