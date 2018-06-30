using System;

using LiDarPointPack;

namespace PointCloudSocket
{
	class MainClass
	{
		private static PclPointCloudPointHandle handle;
		private static PclPointServer server;
		public static void Main (string[] args)
		{
			server = new PclPointServer ();
			// 创建
			handle = new PclPointCloudPointHandle();
			//handle.SendPointsEvent += server.LiDarHandle;
			handle.SendPointsBytesEvent += server.LiDarBytesHandle;


			Console.WriteLine ("Hello World!");
		}
	}
}
