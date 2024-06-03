#include "request_queue.h"

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query,
                                                   DocumentStatus status) {
    return AddFindRequest(
        raw_query, [status](int document_id, DocumentStatus document_status,
                            int rating) { return document_status == status; });
}

std::vector<Document> RequestQueue::AddFindRequest(
    const std::string& raw_query) {
    return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
}

int RequestQueue::GetNoResultRequests() const {
    return std::count_if(
        requests_.begin(), requests_.end(),
        [](const QueryResult& result) { return result.documents_count == 0; });
}