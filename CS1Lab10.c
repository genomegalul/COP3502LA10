#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Trie
{
    struct Trie* children[26];
    int count;
};

struct Trie* createTrie()
{
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    if (pTrie)
    {
        pTrie->count = 0;
        for (int i = 0; i < 26; i++)
            pTrie->children[i] = NULL;
    }
    return pTrie;
}

void insert(struct Trie* pTrie, char* word)
{
    if (pTrie == NULL || word == NULL)
        return;

    struct Trie* currNode = pTrie;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (currNode->children[index] == NULL)
        {
            currNode->children[index] = createTrie();
        }
        currNode = currNode->children[index];
    }
    currNode->count++;
}

struct Trie* findNode(struct Trie* pTrie, char* word)
{
    if (pTrie == NULL || word == NULL)
        return NULL;

    struct Trie* currNode = pTrie;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (currNode->children[index] == NULL)
            return NULL;
        currNode = currNode->children[index];
    }
    return currNode;
}

int numberOfOccurances(struct Trie* pTrie, char* word)
{
    struct Trie* node = findNode(pTrie, word);
    return (node != NULL) ? node->count : 0;
}

struct Trie* deallocateTrie(struct Trie* pTrie)
{
    if (pTrie != NULL)
    {
        for (int i = 0; i < 26; i++)
        {
            pTrie->children[i] = deallocateTrie(pTrie->children[i]);
        }
        free(pTrie);
    }
    return NULL;
}

int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file: %s\n", filename);
        return 0;
    }

    int numWords = 0;
    char buffer[256];

    if (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        numWords = atoi(buffer);
    }

    for (int i = 0; i < numWords && fgets(buffer, sizeof(buffer), file) != NULL; i++)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        pInWords[i] = _strdup(buffer); // Using this version of the function because I am on Visual Studio.
    }

    fclose(file);
    return numWords;
}


int main(void)
{
    char* inWords[256];

    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    char* pWords[] = { "notaword", "ucf", "no", "note", "corg" };
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}
