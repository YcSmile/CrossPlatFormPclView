using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;
using System.Text.RegularExpressions;
using System.Net;
using System.Threading;
using System;
// State object for reading client data asynchronously
public class StateObject {
	// Client  socket.
	public Socket workSocket = null;
	// Size of receive buffer.
	public const int BufferSize = 1024;
	// Receive buffer.
	public byte[] buffer = new byte[BufferSize];
	// Received data string.
	public StringBuilder sb = new StringBuilder();  
	public int id;
}

namespace LiDarPointPack
{
	public class PclPointServer
	{

		// 传输数据
		private Socket server;
		private static List<StateObject> clients;
		private static int id_count = 0;
		private static bool isRuning = false;
		private static Thread serverAccpet;



		//
		public PclPointServer ()
		{
			// 创建服务器端
			clients = new List<StateObject>();
			InitSocket ();

		}
		// 创建TCP服务端
		void InitSocket()
		{
			// 创建Socket Server
			IPEndPoint iep = new IPEndPoint(IPAddress.Any, 8001);
			server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			try{
				server.Bind(iep); 	// 绑定端口
				server.Listen(3);	//

				//server.BeginDisconnect()
				clients = new List<StateObject>();
				isRuning = true;

				// 进行TCP链接监听
				serverAccpet = new Thread(new ThreadStart(ServerThreadAccept));
				serverAccpet.Start();

			}
			catch(Exception e) {
			}
		}
		// 异步监听TCP链接
		public void ServerThreadAccept()
		{
			while (isRuning) {
				server.BeginAccept(new System.AsyncCallback(AcceptCallback),server);
				Thread.Sleep (100);
			}
		}

		// 异步创建链接
		public static void AcceptCallback(IAsyncResult ar) {
			// Signal the main thread to continue.
			//allDone.Set();
			// Get the socket that handles the client request.
			Socket listener = (Socket) ar.AsyncState;
			Socket handler = listener.EndAccept(ar);

			// Create the state object.
			StateObject state = new StateObject();
			state.workSocket = handler;
			state.id = id_count;
			id_count += 1;

			// 异步接收线程
			handler.BeginReceive( state.buffer, 0, StateObject.BufferSize, 0,
				new AsyncCallback(ReadCallback), state);
			// 异步结束
			//handler.BeginDisconnect(false,new AsyncCallback(DisConnt),state);
			//
			lock(clients)
			{
				clients.Add (state);
				Console.WriteLine (clients.Count);
			}
		}

		// 丢失链接
		public static void DisConnt(IAsyncResult ar)
		{
			lock(clients)
			{
				StateObject state = (StateObject)ar.AsyncState;
				//if (clients.Find(StateObject=>state.id==state.id) >= 0) {
				clients.Remove (state);
				//}
				//print("dison");
				Console.WriteLine (clients.Count);
			}
		}

		public static void ReadCallback(IAsyncResult ar) {
			String content = String.Empty;

			// Retrieve the state object and the handler socket
			// from the asynchronous state object.
			StateObject state = (StateObject) ar.AsyncState;
			Socket handler = state.workSocket;

			// Read data from the client socket. 
			int bytesRead = handler.EndReceive(ar);

			if (bytesRead > 0) {
				// There  might be more data, so store the data received so far.
				state.sb.Append(Encoding.ASCII.GetString(
					state.buffer,0,bytesRead));

				// Check for end-of-file tag. If it is not there, read 
				// more data.
				content = state.sb.ToString();
				Console.WriteLine (content);
				if (content.IndexOf("<EOF>") > -1) {
					// All the data has been read from the 
					// client. Display it on the console.
					// Echo the data back to the client.
					Send(handler, content);
				} else {
					// Not all data received. Get more.
					/*
				handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
					new AsyncCallback(ReadCallback), state);
					*/

				}
			}

		}
		// 发送数据
		private static void Send(Socket handler, String data) {
			// Convert the string data to byte data using ASCII encoding.
			byte[] byteData = Encoding.ASCII.GetBytes(data);

			// Begin sending the data to the remote device.
			handler.BeginSend(byteData, 0, byteData.Length, 0,
				new AsyncCallback(SendCallback), handler);

		}
		// 发送数据
		private static void Send(Socket handler, byte[] byteData) {

			// Begin sending the data to the remote device.
			handler.BeginSend(byteData, 0, byteData.Length, 0,
				new AsyncCallback(SendCallback), handler);

		}

		// 发送回调
		private static void SendCallback(IAsyncResult ar) {
			/*
		try {
			// Retrieve the socket from the state object.
			Socket handler = (Socket) ar.AsyncState;

			// Complete sending the data to the remote device.
			int bytesSent = handler.EndSend(ar);
			Console.WriteLine("Sent {0} bytes to client.", bytesSent);

			handler.Shutdown(SocketShutdown.Both);
			handler.Close();

		} catch (Exception e) {
			Console.WriteLine(e.ToString());
		}
		*/
		}
		// 处理数据
		/*
		public void LiDarHandle(float time, LinkedList<SphericalCoordinate> hits){

			//print ("send Liadra data");
			lock (clients) {

				foreach(StateObject state in clients)
				{
					Send (state.workSocket, "1234");

					// 发送数据
				}
			}
		}
		*/
		public void LiDarHandle(PointDataList l_){

			//print ("send Liadra data");
			lock (clients) {
				Console.Write ("1");
				foreach(StateObject state in clients)
				{
					Send (state.workSocket, "1234");
				
					// 发送数据
				}
			}
		}
		public void LiDarBytesHandle(byte[] bs){

			//print ("send Liadra data");
			lock (clients) {
				foreach(StateObject state in clients)
				{
					Send (state.workSocket, bs);
					// 发送数据
				}
			}
		}

	}
}

