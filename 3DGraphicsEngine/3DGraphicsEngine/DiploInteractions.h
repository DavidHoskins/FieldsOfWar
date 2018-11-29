#ifndef DIPLOINTERACTIONS_H
#define DIPLOINTERACTIONS_H

#include <vector>

class DiploInteractions
{
public:
	enum interactionsTypes {alliance, royalMarriage, war, tradeAgreement};

	//Getters and setters.
	std::string getNationID()const {return m_diploNationID;}
	void setNationID(std::string value){m_diploNationID = value;}
	bool getInteractionState(interactionsTypes value){ return m_interactions[value]; }
	void setInteractionState(interactionsTypes type, bool value){m_interactions[type] = value; }
	bool hasTruce() { return (truceTimeLeft > 0); }
	void decrementTruce() { truceTimeLeft--; }
	void setTruceTimer(int value) { truceTimeLeft = value; }
private:
	std::string m_diploNationID;
	bool m_interactions[4];
	int truceTimeLeft = 0;
};

#endif // DIPLOINTERACTIONS_H