#ifndef ELASTICSEARCH_7_10_REQUEST_CREATE_PIT_HPP
#define ELASTICSEARCH_7_10_REQUEST_CREATE_PIT_HPP

#include "adapter/easy/post_adapter.hpp"
#include "adapter/easy/sstream_receiver.hpp"

#include "elasticsearch/v7_10/request/base/base_request.hpp"
#include "elasticsearch/v7_10/answer_model/object/Id.h"
#include "elasticsearch/v7_10/answer_model/pit/object/Pit.h"

#include "elasticsearch/utils/conversion.hpp"

// forward declarations
class Connection;

namespace elasticsearch
{
namespace v7
{
namespace create_pit
{
class transaction: public elasticsearch::base::transaction
{
public:
    using base_t = elasticsearch::base::transaction;

    static constexpr const char *name() { return "v7/create_pit"; }

    transaction(const std::string& host);
    ~transaction();

    template <class Rep, class Period = std::ratio<1>>
    void execute(const std::string& index_name, std::chrono::duration<Rep, Period> keep_alive, bool curl_verbose = false)
    {
        execute(index_name, model::KeepAlive {keep_alive}, curl_verbose);
    }

    void execute(const std::string& index_name, const model::KeepAlive& keep_alive, bool curl_verbose = false);
    model::PIT get_pit() const;
private:
    std::shared_ptr<adapter::easy::StringStreamReceiver> receiver;
    model::KeepAlive last_requested_ka;
};
} // namespace create_pit
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_CREATE_PIT_HPP
