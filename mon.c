#include "mongoose.h"

void uriHandler()
{
	mg_print(conn,
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html\r\n\r\n"
	"<html>\r\n"
	"<body>\r\n"
	"Hello, World!\r\n"
	"</body>\r\n"
	</html>\r\n"
	);
}

int main()
{
	return 0;
}
