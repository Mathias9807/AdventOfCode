import std.stdio;
import std.string;
import std.range;
import std.algorithm;
import core.stdc.stdlib;

byte[127] cardMap;

struct Hand {
	byte[] cards;
	byte[] sorted;
	int bet;
}

byte[] parseHand(string text) {
	byte[] cards;
	cards.length = 5;
	for (int i = 0; i < 5; i++)
		cards[i] = cardMap[text[i]];
	assert(cards.length == 5);
	return cards;
}

int parseNumber(string text) {
	int number = 0;
	foreach (char c; text) {
		if (c < '0' || c > '9') continue;
		number = number * 10 + (c - '0');
	}
	return number;
}

void placeHand(Hand hand, Hand[][] buckets) {
	byte[byte] cardTypes;
	foreach (byte card; hand.cards)
		if (!card in cardTypes) cardTypes[card] = 1;
		else cardTypes[card]++;

	// Switch out all J's with the highest number
	byte highest = 0;
	foreach (byte card; hand.cards) // Find the frequency of the highest card (except jokers)
		if (card != cardMap['J'] && cardTypes[card] > highest)
			highest = cardTypes[card];
	byte highestKey = -1;
	foreach (byte card; hand.cards) // Find the highest card with frequency `highest`
		if (cardTypes[card] == highest && (highestKey == -1 || card > highestKey)) {
			highestKey = card;
		}

	// For each joker
	for (int i = 0; i < hand.sorted.length; i++) {
		byte card = hand.sorted[i];
		if (card != cardMap['J'] || highest == 0) continue;

		hand.sorted[i] = highestKey;
		cardTypes[cardMap['J']]--;
		cardTypes[highestKey]++;
		if (cardTypes[cardMap['J']] == 0) 
			cardTypes.remove(cardMap['J']);
	}
	hand.sorted.sort();
	hand.sorted.reverse();

	// 6: 5 of a kind
	if (canFind(cardTypes.values, 5))
		buckets[6] ~= hand;
	// 5: 4 of a kind
	else if (hand.sorted[0] == hand.sorted[3] || hand.sorted[1] == hand.sorted[4])
		buckets[5] ~= hand;
	// 4: Full house
	else if ((hand.sorted[3] == hand.sorted[4] && hand.sorted[0] == hand.sorted[2]) || (hand.sorted[0] == hand.sorted[1] && hand.sorted[2] == hand.sorted[4]))
		buckets[4] ~= hand;
	// 3: Three of a kind
	else if (hand.sorted[0] == hand.sorted[2] || hand.sorted[4] == hand.sorted[2] || hand.sorted[1] == hand.sorted[3])
		buckets[3] ~= hand;
	// 2: Two pair
	else if (cardTypes.length == 3)
		buckets[2] ~= hand;
	// 1: One pair
	else if (cardTypes.length == 4)
		buckets[1] ~= hand;
	// 0: Randos
	else if (cardTypes.length == 5)
		buckets[0] ~= hand;
}

void main() {
	cardMap['J'] = 1;
	cardMap['2'] = 2;
	cardMap['3'] = 3;
	cardMap['4'] = 4;
	cardMap['5'] = 5;
	cardMap['6'] = 6;
	cardMap['7'] = 7;
	cardMap['8'] = 8;
	cardMap['9'] = 9;
	cardMap['T'] = 10;
	cardMap['Q'] = 12;
	cardMap['K'] = 13;
	cardMap['A'] = 14;

	File file = File("input.txt", "r");

	Hand[] hands;
	while (!file.eof()) {
		string line = file.readln();
		if (line.length == 0) break;
		int num = 0;

		byte[] cards = parseHand(line);
		hands ~= Hand(cards:cards, sorted:cards.dup, bet:parseNumber(line[6..$]));
		hands[$-1].sorted.sort();
		hands[$-1].sorted.reverse();
	}
	writeln("Parsed hands: ", hands);

	Hand[][] buckets; // Sort into buckets by type, then sort each bucket by value
	buckets.length = 7;
	foreach (Hand hand; hands)
		placeHand(hand, buckets);
	// writeln("Placed hands: ", buckets);
	foreach (Hand[] bucket; buckets)
		bucket.sort!((x, y) => cmp(cast(char[]) x.cards, cast(char[]) y.cards) < 0);
	// writeln("Sorted hands: ", buckets);

	int sum = 0, index = 1;
	foreach (Hand[] bucket; buckets)
		foreach (Hand hand; bucket) {
			// writeln("Hand: ", hand, ", Index: ", index);
			sum += hand.bet * index++;
		}
	
	writeln("Answer: ", sum);
}

