/*
 * CardManager.cpp
 *
 *  Created on: Nov 13, 2017
 *      Author: XZ
 */

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <map>
#include <time.h>
#include <iomanip>
#include "CardManager.h"

using namespace std;
#define SHOW_ALL_INDEX (0)
#define SHOW_SELECTED_CARD (1)
#define SHOW_MAP_INFO (0)

//Card Implement
void Card::setCardTypeAndNum(CardType cardType, int cardNum)
{
    assert(cardNum <= 14 && cardNum >= 2);
    m_cardType = cardType;
    m_cardNum  = cardNum;
}
const char* Card::getCardTypeString()const
{
    switch (m_cardType) {
        case CardTypeSpade:
            return "Spade";
        case CardTypeDiamond:
            return "Diamond";
        case CardTypeHeart:
            return "Heart";
        case CardTypeClub:
            return "Club";
        default:
        {
            cout<<"Error Card Type";
            return "Unknown";
        }
            break;
    }
}
const char* Card::getCardNumString()const
{
    static const char* cardNumString[] = {
        "2", "3", "4", "5", "6",
        "7", "8", "9", "10", "J", "Q", "K",
        "A"
    };

    return cardNumString[m_cardNum-2];
}
ostream& operator<< (ostream& os, const Card& cd)
{
    os<<setw(10)<<cd.getCardTypeString()<<" "<<setw(2)<<cd.getCardNumString()<<endl;
    return os;
}
ostream& operator<< (ostream& os, const Card* card)
{
    os<<setw(10)<<card->getCardTypeString()<<" "<<setw(2)<<card->getCardNumString()<<endl;
    return os;
}
//52=13*4
CardManager::CardManager()
{
    int ret = initAllCards();
    assert(ret == true);
    srand((int)time(NULL));
}

CardManager::~CardManager()
{
    for (int i = 0; i<CardNumber; ++i)
    {
        delete m_remainCardsVec[i];
        m_remainCardsVec[i] = NULL;
    }
}
CardManager& CardManager::getInstance()
{
    static CardManager manager;
    return manager;
}
bool CardManager::initAllCards()
{
    Card *pNewCard = NULL;
    for (int i = 0; i < 13; ++i) {
        pNewCard = new Card(CardTypeSpade, i+2);//2-14 spade
        assert(pNewCard);
        m_remainCardsVec.push_back(pNewCard);
    }
    for (int i = 0; i < 13; ++i) {
        pNewCard = new Card(CardTypeHeart, i+2);//2-14 heart
        assert(pNewCard);
        m_remainCardsVec.push_back(pNewCard);
    }
    for (int i = 0; i < 13; ++i) {
        pNewCard = new Card(CardTypeDiamond, i+2);//2-14 diamond
        assert(pNewCard);
        m_remainCardsVec.push_back(pNewCard);
    }
    for (int i = 0; i < 13; ++i) {
        pNewCard = new Card(CardTypeClub, i+2);//2-14 club
        assert(pNewCard);
        m_remainCardsVec.push_back(pNewCard);
    }
    return true;
}

void CardManager::refreshAllCards()//shuffle the deck，1,recycle card from user，2，use swap function
{
    while (!m_recycleCardsVec.empty())
    {
        Card *pOld = m_recycleCardsVec.back();
        m_recycleCardsVec.pop_back();
        m_remainCardsVec.push_back(pOld);
    }
    assert(m_remainCardsVec.size() == CardNumber);

    for (int i = 0;  i< 101; ++i)
    {
        int a = rand() % CardNumber;
        int b = rand() % CardNumber;
        swap(m_remainCardsVec[a], m_remainCardsVec[b]);
    }

#if SHOW_ALL_INDEX
    cout<<"m_allCards:  ";
    for (int i = 0;  i< CardNumber; ++i) {
        cout<<m_remainCardsVec[i]<<" ";
    }
    cout<<endl;
#endif
}
void CardManager::showAllCards()
{
    cout<<"All cards are:"<<endl;
    for (unsigned i = 0; i < m_remainCardsVec.size(); ++i)
    {
        Card* curCard = m_remainCardsVec[i];
        cout<<"    "<<i<<" "<<curCard->getCardNumString()<<" "<<curCard->getCardTypeString()<<endl;
    }
}
Card* CardManager::randomSelectOneCards()
{
    if(m_remainCardsVec.size() <= 0)
        return NULL;
    int cur = rand() % m_remainCardsVec.size();
    Card* pCard = m_remainCardsVec[cur];
    m_remainCardsVec.erase(m_remainCardsVec.begin()+cur);
    m_recycleCardsVec.push_back(pCard);
    return pCard;
}
void CardManager::undoLastSelectOneCards()
{
    if(m_recycleCardsVec.empty())
        return;
    Card* pCard = m_recycleCardsVec.back();
    m_recycleCardsVec.pop_back();
    m_remainCardsVec.push_back(pCard);
}
Card* CardManager::reSelectOneCards()
{
    undoLastSelectOneCards();
    return randomSelectOneCards();
}
void CardManager::undoSelectOneCards(Card *pCard)
{
    VectorCard::iterator it ;
    if(it != m_recycleCardsVec.end())
    {
        m_recycleCardsVec.erase(it);
        m_remainCardsVec.push_back(pCard);
    }
}
Card* CardManager::reSelectOneCardsWithOldCard(Card* pOldCard)
{
    undoSelectOneCards(pOldCard);
    return randomSelectOneCards();
}
VectorCard CardManager::randomSelectCards(int selectNum)
{
    VectorCard selectCards;
    for (int i = 0; i < selectNum; ++i) {
        Card* pCard = randomSelectOneCards();
         //assert(pCard != NULL);
        selectCards.push_back(pCard);
    }

#if SHOW_SELECTED_CARD
    cout<<"Poker selected in his hand: "<<endl;
    for (int i =0; i<selectNum; ++i) {
        cout<<"    "<<selectCards[i];
    }
    cout<<endl;
#endif

    return selectCards;
}


void GamerNormal::showMyCards()
{
    cout<<"Your cards are("<<m_selectCardsVec.size()<<"):"<<endl;
    for (unsigned  i =0; i < m_selectCardsVec.size(); ++i) {
        Card* curCard = m_selectCardsVec[i];
        cout<<curCard->getCardNumString()<<" "<<curCard->getCardTypeString()<<endl;
    }
}
int GamerNormal::showMyCardsValue()
{   int myCardValue;
    for (unsigned  i =0; i < m_selectCardsVec.size(); ++i) {
        Card* curCard = m_selectCardsVec[i];
     myCardValue=(int)curCard->getCardNumString();
    }
   return myCardValue;
}
void GamerNormal::buildCardNumMap()
{
    CardType lastType = CardTypeNone;
    m_isSamesuit = true;
    int maxKey = CardsNumOverMin;
    int minKey = CardsNumOverMax;
    int maxCount = 0;
    m_cardMap.clear();

    for(unsigned  i = 0; i < m_selectCardsVec.size(); ++i)
    {
        Card* curCard = m_selectCardsVec[i];

            m_cardMap[curCard->getNum()]++;
            if(m_cardMap[curCard->getNum()] > maxCount)
            {
                maxCount = m_cardMap[curCard->getNum()];
            }
            CardType curType = curCard->getType();
            if(lastType != CardTypeNone	&& lastType != curType ){
                m_isSamesuit = false;
            }
            lastType = curType;

            if(curCard->getNum() > maxKey)
                maxKey = curCard->getNum();
            if(curCard->getNum() < minKey)
                minKey = curCard->getNum();
    }


    //print: CardNum: numbers
#if SHOW_MAP_INFO
    cout<<"map.count="<<m_cardMap.size()<<endl;
    cout<<"m_isSamesuit="<<m_isSamesuit<<endl;
    map<int,int>::iterator it= m_cardMap.begin();
    for(; it != m_cardMap.end();++it)
    {
        cout<<it->first<<" "<<it->second<<endl;
    }
#endif
}
int GamerNormal::judgeCardsStyle()
{
    /*if(CardsStyleNone != m_cardStyle){
        cout<<"Already judged Cards Style"<<endl;

    }*/

    //cout<<"begin to Judge Cards Style: ";
    buildCardNumMap();

    if(m_cardMap.size() == SELECTCARDNUM)
    {
        bool isOddCard = false; //compute whether the key is consecutive
        int maxKey, minKey;
        getMaxMinKey(maxKey, minKey);
        isOddCard = (maxKey-minKey > 4)? true: false;//like 4，5，6，7，8
        if(isOddCard){
            m_cardStyle = CardsStyleNoPairs;
        }
        else{
        	if(maxKey==14&&minKey==10&&m_isSamesuit){
        	     		 m_cardStyle = CardsStyleRoyalFlush;
        	     	}
        	else if(m_isSamesuit){
        		 m_cardStyle = CardsStyleStraightFlush;
        	}
        	else m_cardStyle = CardsStyleStraight;
        }
    }
    else
    {
    	 if(m_isSamesuit){
    	    	 m_cardStyle = CardsStyleFlush;
    	    }
        int maxCount, minCount;
        getMaxMinCount(maxCount, minCount);

        switch (m_cardMap.size())
        {
            case 2://4+1, 2+3
            {
                if(maxCount == 4)//4+1
                    m_cardStyle = CardsStyleFourOfKind;
                else//2+3
                    m_cardStyle = CardsStyleFullHouse;
            }
                break;
            case 3://2+2+1, 3+1+1
            {
                if(maxCount == 3)
                    m_cardStyle = CardsStyleThreeOfKind;
                else
                    m_cardStyle = CardsStyleTwoPair;
            }
                break;
            case 4://2+1+1+1+1
            {
                m_cardStyle = CardsStyleOnePair;
            }
                break;
            default:
            {
                cout<<"GamerNormal::judgeCardsStyle() error m_cardMap.size(): "<<m_cardMap.size()<<endl;
            }
                break;
        }

    }//else
    return m_cardStyle;
}
void GamerNormal::showMyCardsStyle()
{
    if(CardsStyleNone == m_cardStyle){
        judgeCardsStyle();
    }
    cout<<getCardsStyleString(m_cardStyle)<<"."<<endl;
}

void GamerNormal::getMaxMinKey(int& maxKey, int &minKey)
{
    maxKey = CardsNumOverMin;
    minKey = CardsNumOverMax;
    map<int,int>::iterator it= m_cardMap.begin();
    for(; it != m_cardMap.end();++it)
    {
        if(it->first > maxKey)
            maxKey = it->first;
        if(it->first < minKey)
            minKey = it->first;
    }
}
void GamerNormal::getMaxMinCount(int& maxCount, int &minCount)
{
    maxCount = -1;
    minCount = SELECTCARDNUM;
    map<int,int>::iterator it= m_cardMap.begin();
    for(; it != m_cardMap.end();++it)
    {
        if(it->second > maxCount)
            maxCount = it->second;
        if(it->second < minCount)
            minCount = it->second;
    }
}
const char* getCardsStyleString(CardsStyle style)
{
    static const char* styleString[] = {
        "Unknown",
        "Royal Flush",
        "Straight Flush",
        "Four Of Kind",
        "Full House",
        "Flush",
        "Straight",
        "Three Of Kind",
        "Two Pair",
        "One Pair",
        "No Pairs"
    };

    return styleString[style];
}

