/*
 * CardManager.h
 *
 *  Created on: Nov 13, 2017
 *      Author: XZ
 */

#ifndef CARDMANAGER_H_
#define CARDMANAGER_H_

#include <iostream>
#include <vector>
#include <ostream>
#include <map>
#include <assert.h>
using namespace std;

typedef enum CardType{
    CardTypeSpade=3,
    CardTypeHeart=2,
    CardTypeDiamond=1,
    CardTypeClub=0,
    CardTypeNone=-1,
}CardType;
typedef enum CardsStyle{

    CardsStyleRoyalFlush=1,
    CardsStyleStraightFlush=2,
    CardsStyleFourOfKind=3,
    CardsStyleFullHouse=4,
    CardsStyleFlush=5,
    CardsStyleStraight=6,
    CardsStyleThreeOfKind=7,
    CardsStyleTwoPair=8,
    CardsStyleOnePair=9,
    CardsStyleNoPairs=10,
	CardsStyleNone=11,
}CardsStyle;

typedef enum CardsNum{
    CardsNumOverMin   = 0,
    CardsNumOverMax   = 15,
}CardsNum;

class Card
{
public:
    Card(){ m_cardType=CardTypeNone;
    m_cardNum=0;}
    Card(CardType cardType, int cardNum):m_cardType(cardType), m_cardNum(cardNum)
    {
    }
    void setCardTypeAndNum(CardType cardType, int cardNum);
    const char* getCardTypeString() const;
    const char*  getCardNumString() const;
    CardType getType() const {return m_cardType;}
    int      getNum() const { return m_cardNum;}
public:
    friend ostream& operator<< (ostream& os, const Card& card);
    friend ostream& operator<< (ostream& os, const Card* card);
private:
    CardType m_cardType;
    int      m_cardNum;//2-14 (2-10, J=11,Q=12, K=13,A=14)
};

typedef vector<Card *> VectorCard;
#define SELECTCARDNUM  5

class CardManager
{
public:
    static const int CardNumber=52;

public:
    CardManager();
    ~CardManager();
    CardManager& getInstance();

public:
    void refreshAllCards();
    void showAllCards();
public:
    Card* randomSelectOneCards();
    void undoLastSelectOneCards();
    Card* reSelectOneCards();

    VectorCard randomSelectCards(int selectNum=SELECTCARDNUM);
    void undoSelectOneCards(Card *pCard);
    Card* reSelectOneCardsWithOldCard(Card* pOldCard);

private:
    bool initAllCards();
public:
    VectorCard m_remainCardsVec;
    VectorCard m_recycleCardsVec;
};


class GamerNormal
{
public:
    GamerNormal(const VectorCard& mySelectCards, const string& name="Unknown"):m_selectCardsVec(mySelectCards), m_gamerName(name),m_cardStyle(CardsStyleNone)
    {   m_isSamesuit=false;
        assert(m_selectCardsVec.size() == SELECTCARDNUM);
    }
    ~GamerNormal(){}
public:
    const VectorCard& getMyCards() const {return m_selectCardsVec;}
    const string& getName() const {return m_gamerName;}
    void setSelectCardsVec(const VectorCard& selectCards){
        m_selectCardsVec = selectCards;
        assert(m_selectCardsVec.size() == SELECTCARDNUM);
        m_cardStyle = CardsStyleNone;
    }
public:
    void showMyCards();
    void showMyCardsStyle();
    int showMyCardsValue();
    int judgeCardsStyle();
private:
    void buildCardNumMap();
    void getMaxMinKey(int& maxKey, int &minKey);
    void getMaxMinCount(int& maxCount, int &minCount);
public:    VectorCard m_selectCardsVec;
private:

    map<int, int> m_cardMap;//key is the number on the face of the card，value is the times of the number appears
    string m_gamerName;
    CardsStyle m_cardStyle;
    bool   m_isSamesuit;
};
const char* getCardsStyleString(CardsStyle style);
#endif





