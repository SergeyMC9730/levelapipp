#pragma once

#include "DCommand.h"

#include "Level.h"
#include "cluster.h"
#include "message.h"
#include "snowflake.h"

#include <unordered_map>

#include "SearchFilter.h"

enum SearchInstanceType {
    SearchList = 0,
    SearchID = 1
};

class SearchInstance : public SearchFilter {
public:
    dpp::snowflake _discordUserID;
    dpp::message _discordMessage;
    dpp::cluster *_discordCluster;
    SearchInstanceType _discordType;
    int _levelID;
    std::string _node;

    SearchFilter *getParent();
    SearchInstance *getSelf();
};

class DCommandSearch : public DCommand {
protected:
    DCommandCallback getDefaultCallback() override;
    void eventCallback(std::string name, DCommandEvent *event) override;
public:
    std::unordered_map<dpp::snowflake, SearchInstance> _instances;
public:
    dpp::message render(std::vector<LevelAPI::DatabaseController::Level *> levels, std::string node, int page = -1);

    DCommandSearch(dpp::snowflake botID);
    ~DCommandSearch();

    std::string getCommandName() override;
};