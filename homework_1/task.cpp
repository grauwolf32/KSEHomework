#include <vector>
#include <string>
#include <iostream>

class Date;
class Book;
class User;
class Message;

class LibraryModel;
class LibraryView;
class LibraryController;

const size_t MaxUserBooks = 3;
const size_t MaxDaysToExpire = 30;

using std::string;

enum NotificationType{Default,Error,Notification};

class Date
{
public:

	Date();
	Date(size_t year,size_t month,size_t day);

	size_t year;
	size_t month;
	size_t day;
};

class User{
public:
	User(string name_);
	size_t getUserId() const;
	string getUserName() const;

private:
	size_t id;
	string name;
	std::vector<Book> borrowed_books; 
};


class Book{
public:
	Book(string name);

	size_t getBookId() const;
	string getBookName() const;

	User*  getBookOwner() const;
	void   setBookOwner(User* user);

	void setBorrowingDate(Date& date);
	Date getBorrowingDate() const;

private:
	size_t id;
	string name;
	User* owner;
	Date  borrowing_date;


};

class Message{
public:
	Message();
	Message(NotificationType ntype_,string text_);
	string text;
	NotificationType notification_type;
};

class LibraryModel{
public:
	std::vector<User> getUsers() const;
	std::vector<Book> getBooks() const;

	void addUser(User& user);
	void delUser(User& user);

	void addBook(Book& book);
	void delBook(Book& book);

	void LentABookToUser(Book& book,User& user);
	void TakeABookFromUser(Book& book,User& user);

	std::vector<User> getUsersWithExpiredBooks() const;

private:
	std::vector<User> library_users;
	std::vector<Book> library_books;
	Date current_date;
	LibraryView* view;
};


class LibraryView{
public:
	void ShowAllUsers() const;
	void ShowAllBooks() const;
	void ShowUsersWithExpiredBooks() const;
	void Notify(Message& msg) const;
private:
	LibraryController* controller;
};


class LibraryController{
public:
	void LentABookToUser(Book& book,User& user) { return model->LentABookToUser(book,user); }
	void TakeABookFromUser(Book& book,User& user) { return model->TakeABookFromUser(book,user); }

	std::vector<User> getUsers() const { return model->getUsers(); }
	std::vector<Book> getBooks() const { return model->getBooks(); }

	std::vector<User> getUsersWithExpiredBooks() const { return model->getUsersWithExpiredBooks();}

private:
	LibraryModel* model;
};

Date::Date():year(0),month(0),day(0){}
Date::Date(size_t year_, size_t month_, size_t day_):year(year_),month(month_),day(day_){} //#TODO Assert month <= 12 day <= 31

Message::Message():notification_type(Default),text(string("")) {}
Message(NotificationType ntype_,string text_):notification_type(ntype_),text(text_) {}

User::User(string name){} //

size_t User::getUserId() const { return id; }
string User::getUserName() const { return name; }

size_t Book::getBookId() const { return id; }
User*  Book::getBookOwner() const { return owner; }
string Book::getBookName() const { return name; }

void   Book::setBookOwner(User* user)
{
	owner = user;
}

void Book::setBorrowingDate(Date& date)
{
	borrowing_date.year = date.year;
	borrowing_date.month = date.month;
	borrowing_date.day = date.day;
}

Date Book::getBorrowingDate() const { return borrowing_date; }

void LibraryView::ShowAllUsers() const {
	std::vector<User> users = controller->getUsers();
	if(!users.empty()){
		std::cout << "Users:" << std::endl;
		for(auto iter = users.begin(); iter != users.end(); iter++)
		{
			std::cout << iter->getUserName() << " with id: " << iter->getUserId() << std::endl;
		}
	}
	else {
		std::cout << "There is no users in this library yet." << std::endl;

	}
}

void LibraryView::ShowAllBooks() const {
	std::vector<Book> books = controller->getBooks();
	if(!books.empty()){
		std::cout << "Books:" << std::endl;
		for(auto iter = books.begin(); iter != books.end(); iter++)
		{
			std::cout << iter->getBookName() << " with id: " << iter->getBookId() << std::endl;
		}
	}
	else {
		std::cout << "There is no books in this library yet." << std::endl;

	}
}

void LibraryView::ShowUsersWithExpiredBooks() const{
	std::vector<User> users = controller->getUsersWithExpiredBooks();
	if(!users.empty()){
		std::cout << "Users who expired his/her book(s): " << std::endl;
		for(auto iter = users.begin();iter != users.end(); iter++)
		{
			std::cout << iter->getUserName() << " with id: " << iter->getUserId() << std::endl;
		}
	}
	else{
		std::cout << "There is no users who expired his/her book(s) "<< std::endl;
	}
}

void LibraryView::Notify(Message& msg) const
{
	std::cout << msg.text << std::endl;
}

std::vector<User> LibraryModel::getUsers() const { return library_users; }
std::vector<Book> LibraryModel::getBooks() const { return library_books; }

void LibraryModel::addUser(User& user)
{
	for(auto iter=library_users.begin();iter != library_users.end(); iter++)
	{
		if(iter->getUserId() == user.getUserId()){
			view->Notify(Message(Error,"User already exists!")); 
			return;			
		}
	}
	library_users.push_back(user);
}

void LibraryModel::delUser(User& user)
{
	for(auto iter=library_users.begin();iter != library_users.end();iter++)
	{
		if(iter->getUserId() == user.getUserId())
		{
			library_users.erase(iter);
			return;
		}
	}

	view->Notify(Message(Error,"User does not exists!"));
}

void LibraryModel::addBook(Book& book)
{
	for(auto iter=library_books.begin();iter != library_books.end(); iter++)
	{
		if(iter->getBookId() == book.getBookId())
		{
			view->Notify(Message(Error,"Book already exists!"));
			return;
		}
	}
	library_books.push_back(book);
}

void LibraryModel::delBook(Book& book)
{
	for(auto iter=library_books.begin();iter != library_books.end();iter++)
	{
		if(iter->getBookId() == book.getBookId())
		{
			library_books.erase(iter);
			return;
		}
	}

	view->Notify(Message(Error,"Book does not exists!"));
}

void LibraryModel::LentABookToUser(Book& book,User& user)
{
	if(book.getBookOwner() != (User*)NULL){
		view->Notify(Message(Notification,"This book is aleady owed by someone!"));
		return;
	}

	if(user.borrowed_books.size() > MaxUserBooks)
	{
		view->Notify(Message(Notification,"User owes too much books!"));
		return;
	}

	for(auto iter = user.borrowed_books.begin();iter != user.borrowed_books.end();iter++)
	{
		if(iter->getBookId() == book.getBookId())
		{
			view->Notify(Message(Error,"User user already owes this book!"));
			return;
		}
	}

	user.borrowed_books.push_back(book);
	book.setBookOwner(&user); // Dangerous !!!
	
}


void TakeABookFromUser(Book& book,User& user)
{
	if(book.getBookOwner()->id != user.getUserId())
	{
		view->Notify(Message(Error,"This book owes to another user!"));
		return;
	}

	for(auto iter = user.borrowed_books.begin();iter != user.borrowed_books.end();iter++)
	{
		if(iter->getBookId() == book.getBookId()){
			user.borrowed_books.erase(iter);
			break;
		}
	}
	book.setBookOwner((User*) NULL);
}

std::vector<User> LibraryModel::getUsersWithExpiredBooks() const
{
	size_t current_days = 0;
	size_t lasted_days = 0;

	current_days = current_date.year*365 + current_date.month*30 + current_date.day;

	std::vector<User> expired_users;
	for(auto iter = library_users.begin();iter != library_users.end();iter++)
	{
		for(size_t i = 0;i < iter->borrowed_books.size();i++)
		{
			lasted_days = iter->borrowed_books[i].year*356;
			lasted_days += iter->borrowed_books[i].month*30;
			lasted_days += iter->borrowed_books[i].day;

			if(lasted_days > MaxDaysToExpire){
				expired_users.push_back(*iter);
				break;
			}
		}
	}
	return expired_users;
}



