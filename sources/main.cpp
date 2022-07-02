#include <iostream>
#include "connection/connection.hpp"
#include "connection/multi_connection.hpp"
#include "adapter/easy/sstream_receiver.hpp"
#include "adapter/curlu/url_part_adapter.hpp"
#include "adapter/easy/curlu_adapter.hpp"
#include "adapter/easy/upload_adapter.hpp"
#include "adapter/easy/http/custom_header_adapter.hpp"
#include "adapter/easy/custom_request_adapter.hpp"

#include "elasticsearch/v7_10/request/index_mapping.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "elasticsearch/books/request/create_book.hpp"
#include "elasticsearch/books/request/book_index_mapping.hpp"
#include "elasticsearch/books/request/get_book.hpp"
#include "elasticsearch/books/EPUB/serializer/book_model_serializer.hpp"

#include "elasticsearch/v7_10/request/put_data.hpp"

#include "elasticsearch/v7_10/request/create_pit.hpp"
#include "elasticsearch/v7_10/request/delete_pit.hpp"

//TODO move to request
#include "elasticsearch/books/xml_deserializer.hpp"
#include "elasticsearch/books/json_serializer.hpp"

#include "elasticsearch/books/EPUB/epub_reader.hpp"

int main(int argc, char* argv[])
{
    std::locale::global(std::locale(""));
    curl_global_init(CURL_GLOBAL_ALL);
    CURLcode res;
    /*
    std::string host("http://localhost:9200/_count?pretty");
    adapter::IAdapter::SharedPtr<adapter::easy::StringStreamReceiver> receiver = adapter::IAdapter::create<adapter::easy::StringStreamReceiver>();
    auto get = Connection::create<Connection>(host, receiver);

    res = get->perform();
    if(res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: "
                  << curl_easy_strerror(res) << std::endl;
    }
    else {
        std::cout << receiver->get();
    }

    //multi
    std::cout << "multi" << std::endl;
    receiver->clear();

    auto  m = MultiConnection::create();

    std::string count_url("localhost:9200/_search?pretty=true&q=*:*");
    adapter::IAdapter::SharedPtr<adapter::easy::StringStreamReceiver> receiver_count = adapter::IAdapter::create<adapter::easy::StringStreamReceiver>();
    auto get_count = Connection::create<Connection>(count_url, receiver_count);

    m->add_connection(get);
    m->add_connection(get_count);

    m->wait_all();
    std::cout << receiver->get();
    std::cout << receiver_count->get();

    // curlu
    std::cout << "curlu" << std::endl;
    adapter::IAdapter::SharedPtr<adapter::curlu::URLPartAdapter> curlurl = adapter::IAdapter::create<adapter::curlu::URLPartAdapter>("http://localhost:9200/_count?pretty");
    adapter::IAdapter::SharedPtr<adapter::easy::CurlUAdapter> curlurl_adapter = adapter::IAdapter::create<adapter::easy::CurlUAdapter>(curlurl);
    get->add_adapter(curlurl_adapter);
    get->perform();
    if(res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: "
                  << curl_easy_strerror(res) << std::endl;
    }
    else {
        std::cout << receiver->get();
    }*/

    std::string index("stub_index");
    std::string host("http://localhost:9200/");
    std::string book_path;
    if (argc > 1) {
        index = argv[1];
        if (argc > 2) {
            book_path = argv[2];
        }
    }
    {
        elasticsearch::book::index_mapping::transaction put_index_mapping(host);
        std::cout << "PUT for index: " << index << std::endl;
        try
        {
            put_index_mapping.execute(index, true);
            auto ans = put_index_mapping.get_response();
            (void)ans;
        }
        catch (const std::exception &ex)
        {
            std::cout << "PUT failed: " << ex.what() << std::endl;
        }
        std::cout << "PUT Response:\n" << put_index_mapping.get_receiver().get();

        //TODO repeat again to get bad response
        try
        {
            put_index_mapping.execute(index, true);
            auto ans = put_index_mapping.get_response();
            (void)ans;
        }
        catch (const std::exception& ex)
        {
            std::cout << "PUT Duplicate failed: " << ex.what() << std::endl;
        }
        std::cout << "PUT Duplicate Response:\n" << put_index_mapping.get_receiver().get();
        // PIT
        {
            std::cout << "Create PIT " << std::endl;
            elasticsearch::v7::create_pit::transaction create(host);
            create.execute(index, std::chrono::seconds(10), true);
            elasticsearch::v7::Id pit = create.get_pit();
            std::cout << "PIT created: " << pit.value() << std::endl;

            std::cout << "Delete PIT " << std::endl;
            elasticsearch::v7::delete_pit::transaction destroy(host);
            destroy.execute(pit, true);
            auto resp = destroy.get_response();

            elasticsearch::v7::Id pit_bad(create.get_pit().value() + "BAD");
            destroy.get_receiver().clear();
            destroy.execute(pit_bad, true);
            auto bad_resp = destroy.get_response();
        }
        // Insert book
        if (!book_path.empty()) {

            txml::StdoutTracer std_tracer;
            epub::reader d(book_path);
            auto &&epub_model_header = d.getOPF();
            elasticsearch::book::epub::to_model_data b2m;
            epub_model_header->format_serialize(b2m, std_tracer);

            elasticsearch::book::create::transaction create_book(host, *b2m.data_model);
            std::cout << "PUT book for index: " << index << std::endl;
            try
            {
                create_book.execute(index + "/_doc/1", true);
                auto& receiver = create_book.get_receiver();
                auto ans = create_book.get_response();
                (void)ans;
            }
            catch (const std::exception &ex)
            {
                std::cout << "transaction failed: " << ex.what() << std::endl;
            }
            std::cout << "transaction Response:\n" << create_book.get_receiver().get();


            elasticsearch::book::get::transaction get_book(host);
            std::cout << "GET book for index: " << index << std::endl;
            try
            {
                get_book.execute(index + "/_doc/1", true);
                auto ans = get_book.get_response();
                (void)ans;
            }
            catch (const std::exception &ex)
            {
                std::cout << "transaction failed: " << ex.what() << std::endl;
            }
            std::cout << "transaction Response:\n" << get_book.get_receiver().get();
        }

        std::string index_next = index + "_next";
        std::cout << "PUT for index_next: " << index_next << std::endl;
        try
        {
            put_index_mapping.execute(index_next, true);
        }
        catch (const std::exception &ex)
        {
            std::cout << "PUT failed: " << ex.what() << std::endl;
        }
        std::cout << "PUT Response:\n" << put_index_mapping.get_receiver().get();


        elasticsearch::v7::index_mapping_delete::transaction delete_index_mapping(host);
        std::cout << "DELETE for index: " << index << std::endl;
        try
        {
            delete_index_mapping.execute(index, true);
        }
        catch (const std::exception &ex)
        {
            std::cout << "DELETE failed: " << ex.what() << std::endl;
        }
        std::cout << "DELETE Response:\n" << delete_index_mapping.get_receiver().get();
        std::cout << "DELETE for index_next: " << index_next << std::endl;
        try
        {
            delete_index_mapping.execute(index_next, true);
        }
        catch (const std::exception &ex)
        {
            std::cout << "DELETE failed: " << ex.what() << std::endl;
        }
        std::cout << "DELETE Response:\n" << delete_index_mapping.get_receiver().get();
    }
    // put
/*
    {
        std::cout << "PUT for index: " << index << std::endl;
        adapter::IAdapter::SharedPtr<adapter::curlu::URLPartAdapter> url = adapter::IAdapter::create<adapter::curlu::URLPartAdapter>("http://localhost:9200/");
        adapter::IAdapter::SharedPtr<adapter::easy::CurlUAdapter> url_adapter = adapter::IAdapter::create<adapter::easy::CurlUAdapter>(url);
        adapter::IAdapter::SharedPtr<adapter::easy::StringStreamReceiver> receiver = adapter::IAdapter::create<adapter::easy::StringStreamReceiver>();

        std::string fields {"{\n"
                                                "\"mappings\": {\n"
                                                    "\"properties\": {\n"
                                                        "\"name\": {\n"
                                                            "\"type\": \"text\"\n"
                                                        "},\n"
                                                        "\"blob\": {\n"
                                                            "\"type\": \"binary\"\n"
                                                        "}\n"
                                                    "}\n"
                                                "}\n"
                                               "}\n"
                                            };
        std::vector<unsigned char> put_data(fields.size(), 0);
        std::copy(fields.begin(), fields.end(), put_data.begin());

        // to send PUT
        adapter::IAdapter::SharedPtr<adapter::easy::UploadAdapter> uploader = adapter::IAdapter::create<adapter::easy::UploadAdapter>(std::move(put_data));

        auto put_index_mapping = Connection::create<Connection>("", url_adapter, receiver, uploader);

        // set custom path
        url->set_path(index + "?pretty");

        // set headers
        auto header = adapter::IAdapter::create<adapter::easy::http::CustomHeader>(std::initializer_list<const char*>{"Content-Type: application/json","Expect:"});
        put_index_mapping->add_adapter(header);

        // send
        curl_easy_setopt(put_index_mapping->get_handle(), CURLOPT_VERBOSE, 1L);
        put_index_mapping->perform();
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: "
                    << curl_easy_strerror(res) << ", code: " << res <<  std::endl;
        }
        std::cout << "RESP:\n" << receiver->get();


        //////////////////////////////////////////////////////
        //DELETE
        receiver->clear();

        put_index_mapping->remove_adapter(uploader);
        put_index_mapping->remove_adapter(header);
        auto custom = adapter::IAdapter::create<adapter::easy::CustomRequestAdapter>("DELETE");
        put_index_mapping->add_adapter(custom);
        url->set_path(index + "?pretty");
        put_index_mapping->perform();
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: "
                    << curl_easy_strerror(res) << ", code: " << res <<  std::endl;
        }
        std::cout << "RESP:\n" << receiver->get();
    }
*/

    curl_global_cleanup();
    return 0;
}
