#include "DataBase.h"

#include <map>
#include <list>
#include <assert.h>
#include <iostream>
#include <fstream>

#include "User.h"
#include "Message.h"

namespace
{
	// База зарегистрированных пользователей
	// где: string - id пользователя(login); User - структура с данными пользователя (ник-логин-пароль)
	std::map<std::string, User> all_users;

	// Имя адресата чтобы отправить сообщение всем
	const std::string MSG_TO_ALL = "all";

	// База сообщений пользователей друг другу
	std::list<Message> all_messages;
}


void database::initialize()
{
	User u1("G", "Ger", "123");
	User u2("S", "Sve", "qwe");

	std::pair<std::string, User> new_pair = std::make_pair("Ger", u1);
	all_users.insert(new_pair);

	std::pair<std::string, User> new_pair2 = std::make_pair("Sve", u2);
	all_users.insert(new_pair2);
}


bool database::isExistLogin(const std::string &login)
{
	auto result = all_users.find(login);
	if (result != all_users.end())
	{
		return true;
	}
	return false;
}


bool database::isExistName(const std::string &name)
{
	// Range-based loop
	for (const auto &user : all_users)
	{
		if (user.second.getName() == name)
		{
			return true;
		}
	}
	return false;
}


bool database::isCorrectPassword(const std::string &login, const std::string &password)
{
	if (isExistLogin(login))
	{
		if (all_users.at(login).getPassword() == password)
		{
			return true;
		}
	}
	return false;
}


void database::pushMessage(const Message &message)
{
	all_messages.push_back(message);
}


void database::loadMessages(const User &addressee, std::shared_ptr<std::list<Message>> destination)
{
	destination->clear();
	// Пройти по сообщениям в базе
	for (const auto &message : all_messages)
	{
		// Если адресат сообщения совпадает с заданным пользователем
		// или сообщение ВСЕМ
		if (message.getNameTo() == addressee.getName() ||
			(message.getNameTo() == MSG_TO_ALL))
		{
			destination->push_back(message);
		}
	}
}


void database::addUser(const User &user)
{
	// Если пользователь ещё не в базе данных
	const std::string login = user.getLogin();
	if (!isExistLogin(login))
	{
		std::pair<std::string, User> new_pair = std::make_pair(login, user);
		all_users.insert(new_pair);
	}
}


/**
Удалить из базы все сообщения, адресованные пользователю с заданным ником
\param[in] name Ник пользователя
*/
static void removeMessagesToUser(const std::string &name);


void database::removeUser(const User &user)
{
	removeMessagesToUser(user.getName());
	all_users.erase(user.getLogin()); // удалить по логину
}


std::string database::getNameByLogin(const std::string &login)
{
	if (isExistLogin(login))
	{
		return all_users.at(login).getName();
	}
	return "";
}


size_t database::getNumberUser()
{
	return all_users.size();
}


void database::loadUserNames(std::shared_ptr<std::vector<std::string>> userNames)
{
	userNames->clear();
	for (const auto &user : all_users)
	{
		userNames->push_back(user.second.getName());
	}
}


static void removeMessagesToUser(const std::string &name)
{
	// Удалить сообщение по условию true
	all_messages.remove_if([&name](const Message &message)
						   {
		//Если адресат текущего сообщения совпадает с заданным именем
		if (message.getNameTo() == name) {
			return true;
		}
		return false; });
}