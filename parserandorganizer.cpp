
    #include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::pair;

int main(int argc, char **argv) {
  // Recommendation: Define your associative containers here.
  // Use enough containers such that you only need to parse the input data one time in a "single pass".
  std::map<std::string, int> authors_comments;
  std::vector<pair<std::string, int>> ac;

  std::map<std::string, pair<int, int>> authors_utc;

  std::map<std::string, int> author_total_bodysize;
  std::vector<pair<std::string, int>> acount;

  std::map<std::string, int> author_longest_bodysize;
  std::vector<pair<std::string, int>> lvec;

  std::map<std::string, std::set<std::string>> authors_by_subreddit;
  std::vector<pair<std::string, std::set<std::string>>> authors_by_subreddit_vec;

  // Bonus
  std::map<std::string, int> firstwordcount;
  std::vector<pair<std::string, int>> fwc_vec;

  std::map<std::string, std::set<std::string>> author_subreddits;
  std::vector<std::pair<std::string, std::set<std::string>>> as;

  // Map of subreddits and vector of comments
  std::map<std::string, std::vector<std::string>> subreddit_comments;

  // Vector of subreddits and vector of comments
  std::vector<std::pair<std::string, std::vector<std::string>>> subreddit_comments_vec;
  std::vector<std::string> subreddit_comments_vec10;

  // Map of words in the top10 subreddits and their count
  std::map<std::string, int> top10words;

  // Vector of words in the top10 subreddits and their count
  std::vector<std::pair<std::string, int>> top10words_vec;

  // Note: The following code parses the reddit ndjson input file.
  // You should not need to modify this code.

  // Parse the reddit ndjson input file
  std::ifstream ifs("/mnt/c/Users/jmayh/Documents/RedditDataProject/RC_2019-02-01_trimmed.ndjson");
  std::string line;
  size_t record_count{0};
  while (std::getline(ifs, line)) {
    record_count++;
    // Use this to help test faster with fewer records
    // if (record_count == 5000) break;

    // Important fields to use
    auto json = nlohmann::json::parse(line);
    std::string author = json["author"];
    std::string subreddit = json["subreddit"];
    std::string body = json["body"];
    std::string id = json["id"];
    std::string parent_id = json["parent_id"];
    size_t created_utc = json["created_utc"];
    size_t score = json["score"];
    std::stringstream s(body);
    std::string firstword = "";
    s >> firstword;

    // Add data to associative containers using the fields above
    // ...

    // Bonus: firstwordcount
    firstwordcount[firstword] += 1;

    // Problem 1 and Problem 2 and Q3
    authors_comments[author] += 1;


    //Problem 5
    if(author_longest_bodysize[author] < body.size())
    {
      author_longest_bodysize[author] = body.size();
    }
    // Problem 4
    //if the current utc time is less than current lowest replace it
    if(created_utc < authors_utc[author].first){
      authors_utc[author].first = created_utc;
    }
    else if(authors_utc[author].first == 0){
      authors_utc[author].first = created_utc;
    }
           if(created_utc > authors_utc[author].second){
      authors_utc[author].second = created_utc;
    }
    //if the current utc time is more than current highest replace it
  
    //question6
    author_total_bodysize[author] += body.size();
    //Problem 7 and 8
    author_subreddits[author].insert(subreddit);
     //Problem 9 and 10
    authors_by_subreddit[subreddit].insert(author);
    //Problem 13
    subreddit_comments[subreddit].push_back(body);
  
  }
    cout << record_count << " records parsed." << endl;

  //
  // Notes:
  // When ties in authors or subreddits exists, lexigraphically sort by string.
  //
 for(const auto& thing : authors_comments)
  {
    //put everything into vector
    ac.push_back(thing);

  }
  //
 
// 1. Who are the Top 10 most prolific authors (most number of comments)?
// sort vector ac in decreasing order

// Once in the vector, now sort it
std::sort(ac.begin(), ac.end(),
  [](const auto &p1, const auto &p2) {return p1.second > p2.second;});

cout << "1. These are the top 10 most commenting authors" << endl;
{
  int count = 0;
  for (const auto& thing : ac) {
    if (count == 10) break;
    cout << "The author " << thing.first << " commented " << thing.second << endl;
    count++;
  }
}


// 2. How many authors only comment once?

cout << "2. This many authors only comment once" << endl;
// Resort the vector from question 1 in reverse order
std::sort(ac.begin(), ac.end(),
  [](const auto &p1, const auto &p2) {return p1.second < p2.second;});
// Keep count
{
  int count = 0;
  for (const auto& thing : ac) {
    if (thing.second != 1) break;
    count++;
  }
  cout << count << endl;
}


// 3. Which author makes the most total comments?

std::sort(ac.begin(), ac.end(),
  [](const auto &p1, const auto &p2) {return p1.second > p2.second;});
cout << "3. The author that makes the most total comments is" << endl;
cout << "The author: " << ac.front().first << " has made " << ac.front().second << " comments" << endl;


// 4. For the author with the most comments, what is the min/max UTC time?
cout << "4. The author with the most comments, min and max UTC time is " << ac.front().first << " for the lowest UTC time: " << authors_utc[ac.front().first].first << " and with the highest UTC time: " << authors_utc[ac.front().first].second << endl;


// 5. Which author makes the author_longest_bodysize comment? Hint: body.size()
for (const auto &thing : author_longest_bodysize) {
  lvec.push_back(thing);
}
std::sort(lvec.begin(), lvec.end(),
  [](const auto &p1, const auto &p2) {return p1.second > p2.second;});

cout << "5. The author that makes the author_longest_bodysize comment is " << lvec.front().first << " and it is this many words long: " << lvec.front().second << endl;


// 6. Which author types the most? (assume body.size() is how much the author typed.
for (const auto& thing : author_total_bodysize) {
  // Put everything into a vector
  acount.push_back(thing);
}
cout << "6. The author that types the most" << endl;

std::sort(acount.begin(), acount.end(),
  [](const auto &p1, const auto &p2) {return p1.second > p2.second;});
cout << "The author: " << acount.front().first << " has typed this many words: " << acount.front().second << endl;


  //
  // 7. Which author comments on the most number of subreddits?
// loop through and get each sets contstns in the mapo

  std::cout << "7.the author that comments on the most subreddits " << endl;
  for(const auto& thing : author_subreddits){
    //cout << thing.first << " " << thing.second.size() << endl;
    as.push_back(thing);
  }
  //sort the vector
  std::sort(as.begin(), as.end(), [](const auto &p1, const auto &p2){return p1.second.size() > p2.second.size();});
  {
      int count = 0;
  for(const auto& thing : as){
    if(count == 1) break;
    cout << thing.first << " " << thing.second.size() << endl;
    count++;
  }
  }



  //
  // 8. How many authors only comment on 1 subreddit?
{

    //sort the vector
  std::sort(as.begin(), as.end(), [](const auto &p1, const auto &p2){return p1.second.size() < p2.second.size();});
  //create a count
  int count = 0;
  for(const auto& thing : as)
  {
    if(thing.second.size() == 1) count++;
  }
  cout << "8. this many authors only commented on 1 subreddit: " << count << endl;
}
  //
  // 9. How many subreddits only have 1 author?
  {
//push everything in the map to a pair vector
for(const auto& thing : authors_by_subreddit){
  authors_by_subreddit_vec.push_back(thing);
}

//sort the vector
  std::sort(authors_by_subreddit_vec.begin(), authors_by_subreddit_vec.end(), [](const auto &p1, const auto &p2){return p1.second.size() < p2.second.size();});
  int count = 0;
  for(const auto& thing : authors_by_subreddit_vec){
    if(thing.second.size() == 1)
    count ++;
  }
  cout << "9. This many subreddits have 1 author: " << count << endl;
  }

  //
  // 10. Which subreddit has the most authors?
  {
    int most_auto = 0;
    std::string sub_mostauthors="";
      for(const auto& thing: authors_by_subreddit_vec){
        if(thing.second.size() > most_auto){
            sub_mostauthors = thing.first;
            most_auto = thing.second.size();
        }

  }
  cout << "10. the subreddit with the most authors is " << sub_mostauthors << ", which has " << most_auto << endl;
  }

  //
  // 12. Which word is the most frequently used to start a comment?
  //     (Assume words are just characters separated by whitespace)
for(const auto &thing : firstwordcount)
{
  fwc_vec.push_back(thing);

}
//nowsort
std::sort(fwc_vec.begin(), fwc_vec.end(),
  [](const auto &p1, const auto &p2) {return p1.second > p2.second;});
  std::cout << "12. the word most freq used to start a comment is, " << fwc_vec.front().first << endl;
  //
  // 13. For the top 10 commented on subreddits, what is the most used word?
for(const auto &thing : subreddit_comments)
{
  subreddit_comments_vec.push_back(thing);
}
//sort subreddit_comments_vec from greatest size of vector to least
std::sort(subreddit_comments_vec.begin(), subreddit_comments_vec.end(),
  [](const auto &p1, const auto &p2) {return p1.second.size() > p2.second.size();});

 //go through the frist 10 vectors in subreddit_comments_vec and add all the strings to a vector subreddit_comments_vec10
  for(int i = 0; i < 10; i++)
  {
    for(const auto &thing : subreddit_comments_vec[i].second)
    {
      subreddit_comments_vec10.push_back(thing);
    }
  }
  //now go through each string in subreddit_comments_vec10 and add each word to map top10words

  for(const auto &thing : subreddit_comments_vec10)
  {
    std::istringstream iss(thing);
    std::string word;
    while(iss >> word)
    {
      top10words[word]++;
    }
  }
//put each pair in map top10words into a vector top10words_vec
  for(const auto &thing : top10words)
  {
    top10words_vec.push_back(thing);
  }
//sort top10words_vec from greatest to least using the int
  std::sort(top10words_vec.begin(), top10words_vec.end(),
  [](const auto &p1, const auto &p2) {return p1.second > p2.second;});
  //return the first
  std::cout << "13. the most used word in the top 10 subreddits is, " << top10words_vec.front().first << endl;
  
 
}