#ifndef RELATIONSHIPHANDLER_H
#define RELATIONSHIPHANDLER_H

#include <vector>

#include "DiploInteractions.h"

class UIHandler;
class NationHandler;
class Nation;

class RelationshipHandler
{
public:
	UIHandler* m_UI;
	std::vector<DiploInteractions*> m_diploInteractionsState;
	std::vector<std::string> m_diploID;
	~RelationshipHandler();
	RelationshipHandler();

	void Update(NationHandler* nations);

	void setRelationshipByType(Nation* thisNation, Nation* otherNation, DiploInteractions::interactionsTypes interaction, bool value);
	bool getRelationshipState(Nation* otherNation, DiploInteractions::interactionsTypes interaction);
private:

};
#endif //!RELATIONSHIPHANDLER_H 

