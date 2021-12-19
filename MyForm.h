#pragma once

#include <winsock.h>
#include <stdio.h>

#define WSA_UDP_NETEVENT	(WM_USER+1)
#define	WSA_TCP_NETEVENT	(WM_USER+2)
#define	WSA_NETACCEPT		(WM_USER+3)

#define SERVER_PORT_TCP     0x2021
#define CLIENT_PORT_TCP     0x2031

#define SERVER_PORT_UDP     0x2001
#define CLIENT_PORT_UDP     0x2011

SOCKET         UDPSocket, TCPSocket, TmpSocket;      
sockaddr_in	   CallAddress;
sockaddr_in	   OurAddress;

int			   f = 0;

namespace Practive5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^ listBox1;
	protected:
	private: System::Windows::Forms::ListBox^ listBox2;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ button1;

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->listBox2 = (gcnew System::Windows::Forms::ListBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(12, 12);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(198, 251);
			this->listBox1->TabIndex = 0;
			// 
			// listBox2
			// 
			this->listBox2->FormattingEnabled = true;
			this->listBox2->Location = System::Drawing::Point(216, 12);
			this->listBox2->Name = L"listBox2";
			this->listBox2->Size = System::Drawing::Size(198, 199);
			this->listBox2->TabIndex = 1;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(216, 217);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(198, 20);
			this->textBox1->TabIndex = 2;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(216, 243);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(198, 23);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Отправить";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(452, 288);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->listBox2);
			this->Controls->Add(this->listBox1);
			this->Name = L"MyForm";
			this->Text = L"SERVER";
			this->Activated += gcnew System::EventHandler(this, &MyForm::MyForm_Activated);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MyForm::MyForm_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Activated(System::Object^ sender, System::EventArgs^ e) {
		WSADATA	    WSAData;
		int		    rc;
		char		Name[101], * IpAddr, Buf[1000];
		PHOSTENT    phe;


		if (f == 1) return;
		f = 1;
		rc = WSAStartup(MAKEWORD(2, 0), &WSAData);
		if (rc != 0) {
			listBox1->Items->Add("Ошибка инициализации WSAStartup");
			return;
		} 


		//UDP
		UDPSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (UDPSocket == INVALID_SOCKET) {
			listBox1->Items->Add(L"Протокол UDP не установлен.");
		} 

		memset(&OurAddress, 0, sizeof(OurAddress));
		OurAddress.sin_family = AF_INET;
		OurAddress.sin_port = SERVER_PORT_UDP;

		rc = bind(UDPSocket, (LPSOCKADDR)&OurAddress, sizeof(sockaddr_in));
		if (rc == SOCKET_ERROR) {
			listBox1->Items->Add("Адресная ошибка");
			return;
		} 

		rc = WSAAsyncSelect(UDPSocket, (HWND)(this->Handle.ToInt32()),WSA_UDP_NETEVENT, FD_READ);
		if (rc != 0) {
			listBox1->Items->Add("Ошибка WSAAsyncSelect");
			return;
		} 
		listBox1->Items->Add(L"Протокол UDP установлен.");


		//TCP
		TCPSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (TCPSocket == INVALID_SOCKET) {
			listBox1->Items->Add(L"Протокол TCP не установлен.");
		} 

		memset(&OurAddress, 0, sizeof(OurAddress));
		OurAddress.sin_family = AF_INET;
		OurAddress.sin_port = SERVER_PORT_TCP;

		rc = bind(TCPSocket, (LPSOCKADDR)&OurAddress, sizeof(sockaddr_in));
		if (rc == SOCKET_ERROR) {
			listBox1->Items->Add("Адресная ошибка");
			return;
		}

		listBox1->Items->Add(L"Протокол TCP установлен.");

		rc = WSAAsyncSelect(TCPSocket, (HWND)(this->Handle.ToInt32()), WSA_NETACCEPT, FD_ACCEPT);
		if (rc != 0) {
			listBox1->Items->Add("Ошибка WSAAsyncSelect");
			return;
		}

		rc = listen(TCPSocket, 1);
		if (rc == SOCKET_ERROR) {
			listBox1->Items->Add("Ошибка listen");
			return;
		}

		gethostname(Name, 101);
		strcpy(Buf, "Имя компьютера ");
		strcat(Buf, Name);
		String^ s = gcnew String(Buf);
		listBox1->Items->Add(s);

		phe = gethostbyname(Name);
		if (phe != NULL) {
			memcpy((void*)&(OurAddress.sin_addr), phe->h_addr, phe->h_length);
			IpAddr = inet_ntoa(OurAddress.sin_addr);
			strcpy(Buf, "IP-Адрес ");
			strcat(Buf, IpAddr);
			String^ s2 = gcnew String(Buf);
			listBox1->Items->Add(s2);
		} 

		listBox1->Items->Add(L"Сервер запущен");
	}

	protected: virtual void WndProc(Message% m) override
	{
		int      rc, l = sizeof(CallAddress);
		wchar_t  Buf[500];

		if (m.Msg == WSA_NETACCEPT) {
			if (m.LParam.ToInt32() == FD_ACCEPT) {
				TmpSocket = accept((SOCKET)m.WParam.ToInt32(), (PSOCKADDR)&CallAddress, (int*)&l);
				if (TmpSocket == INVALID_SOCKET) {
					rc = WSAGetLastError();
					listBox1->Items->Add(String::Format("Ошибка accept {0}", rc));
					return;
				}
				rc = WSAAsyncSelect(TmpSocket, (HWND)(this->Handle.ToInt32()), WSA_TCP_NETEVENT, FD_READ);
				if (rc != 0) {
					listBox1->Items->Add("Ошибка WSAAsyncSelect");
					return;
				}
				listBox1->Items->Add("Канал создан");
			}
		}

		if (m.Msg == WSA_UDP_NETEVENT) {
			if (m.LParam.ToInt32() == FD_READ) {
				rc = recvfrom((SOCKET)m.WParam.ToInt32(), (char*)Buf, sizeof(Buf) - 1, 0, (PSOCKADDR)&CallAddress, &l);
				if (rc == SOCKET_ERROR) {
					rc = WSAGetLastError();
					listBox1->Items->Add(String::Format("Ошибка recv {0}", rc));
					return;
				} 
				if (rc >= 1) {
					String^ s = gcnew String(Buf);
					listBox2->Items->Add(s);
				} 

				rc = sendto(UDPSocket, "2", 4, 0, (PSOCKADDR)&CallAddress, sizeof(CallAddress));
				if (rc == SOCKET_ERROR) {
					rc = WSAGetLastError();
					listBox1->Items->Add(String::Format("Ошибка sendto {0}", rc));
					return;
				}
				listBox1->Items->Add("2");

			}
		} 

		if (m.Msg == WSA_TCP_NETEVENT) {
			if (m.LParam.ToInt32() == FD_READ) {
				rc = recv((SOCKET)m.WParam.ToInt32(), (char*)Buf, sizeof(Buf) - 1, 0);
				if (rc == SOCKET_ERROR) {
					rc = WSAGetLastError();
					listBox1->Items->Add(String::Format("Ошибка recv {0}", rc));
					return;
				} 
				if (rc >= 1) {
					String^ s = gcnew String(Buf);
					listBox1->Items->Add(s);
				} 
			}
			else {
				listBox1->Items->Add("Канал разорван");
			} 
		} 

		Form::WndProc(m);
	}

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		int      rc, i, l;
		wchar_t  Buf[501];

		l = textBox1->Text->Length;
		if (l == 0) return;
		for (i = 0; i < l; i++) {
			Buf[i] = textBox1->Text->default[i];
			Buf[i + 1] = 0;
		} 

		rc = send(TmpSocket, (char*)Buf, 2 * l + 2, 0);
		if (rc == SOCKET_ERROR) {
			rc = WSAGetLastError();
			listBox1->Items->Add(String::Format("Ошибка send {0}", rc));
			return;
		}
		listBox2->Items->Add(textBox1->Text);
	}
private: System::Void MyForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {
	closesocket(TCPSocket);
	closesocket(UDPSocket);
	closesocket(TmpSocket);
}
};
}
