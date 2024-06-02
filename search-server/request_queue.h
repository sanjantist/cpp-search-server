#pragma once

#include <algorithm>
#include <deque>

#include "search_server.h"

class RequestQueue {
   public:
    explicit RequestQueue(const SearchServer& search_server)
        : server_(search_server) {}
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query,
                                         DocumentPredicate document_predicate);
    std::vector<Document> AddFindRequest(const std::string& raw_query,
                                         DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;

   private:
    struct QueryResult {
        int minute;
        std::vector<Document> result;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& server_;
};

template <typename DocumentPredicate>
inline std::vector<Document> RequestQueue::AddFindRequest(
    const std::string& raw_query, DocumentPredicate document_predicate) {
    QueryResult result;

    if (requests_.empty()) {
        result.minute = 1;
    } else {
        result.minute = requests_.back().minute + 1;
    }

    std::vector<Document> find_request_result =
        server_.FindTopDocuments(raw_query, document_predicate);
    result.result = find_request_result;

    if (requests_.size() != min_in_day_) {
        requests_.push_back(result);
    } else {
        requests_.pop_front();
        requests_.push_back(result);
    }

    return find_request_result;
}