#include "common.h"

boost::asio::streambuf BUF_;

void read_buf(boost::asio::local::stream_protocol::socket& s,boost::asio::streambuf& buf)
{
    size_t size = 0;
    //read length
    boost::asio::read(s,boost::asio::buffer(&size,sizeof(size_t)));
    if(size == 0){
        std::cout<<"No data..."<<std::endl;
        return;
    }
    //read content
    boost::asio::read(s,buf,boost::asio::transfer_exactly(size));
}

void write_buf(boost::asio::local::stream_protocol::socket& s,boost::asio::streambuf& buf)
{
    size_t size = buf.size();
    //first send length.
    boost::asio::write(s,boost::asio::buffer(&size,sizeof(size_t)));
    //then send content
    boost::asio::write(s,buf,boost::asio::transfer_exactly(size));
}

void finished_handler(boost::asio::local::stream_protocol::socket& s,const boost::system::error_code& ec)
{
    if(!ec){
        s.shutdown(boost::asio::socket_base::shutdown_send);
        boost::asio::async_read(s,BUF_,
                boost::bind(&finished_handler,boost::ref(s),boost::asio::placeholders::error));
    }else if(ec == boost::asio::error::eof){
        s.shutdown(boost::asio::socket_base::shutdown_receive);
        s.close();
    }
}

int main(int argc, char *argv[])
{
    try{
        if(argc != 2){
            std::cerr <<"%s path"<<std::endl;
            return 1;
        }

        boost::asio::io_service service;

        boost::asio::local::stream_protocol::socket s(service);
        service.run();

        s.connect(boost::asio::local::stream_protocol::endpoint(argv[1]));

        info::Info msg;
        msg.set_id(2);
        msg.set_name("hello,world.");

        //write msg.
        boost::asio::streambuf buf1;
        std::ostream os(&buf1);
        msg.SerializeToOstream(&os);
        write_buf(s,buf1);

        //read msg size
        boost::asio::streambuf buf2;
        read_buf(s,buf2);

        //反序列化
        info::Message_t msgs;
        std::istream in(&buf2);
        msgs.ParseFromIstream(&in);

        info::Info *p;
        for(int i=0;i<msgs.msg_size();++i){
            p = msgs.mutable_msg(i);
            std::cout<<p->id()<<","<<p->name()<<std::endl;
        }

        //等待关闭信号
        boost::system::error_code e;
        finished_handler(s,e);

    }catch(std::exception& e){
        std::cout<<"except:"<<e.what()<<std::endl;
    }

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
