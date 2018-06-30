using System;
using System.Threading;
using System.IO;
using LiDarPointPack;
using System.Runtime.Serialization.Formatters.Binary;
using System.Collections.Generic;
using Google.Protobuf;
using System.Diagnostics;
namespace PointCloudSocket
{

	public class PclPointCloudMessage
	{
		public long length;
		public byte[] data;
		public byte[] long2bytes(long num)
		{
			return BitConverter.GetBytes (num);
		}

		public byte[] MsgFormat()
		{
			List<byte> byteSource = new List<byte>();
			byteSource.AddRange (long2bytes (length));
			byteSource.AddRange (data);
			return byteSource.ToArray ();
		}
	}

	public class PclPointCloudPointHandle
	{

		private Thread genThread;
		private int sleepTime = 100;
		public delegate void PclSendPoints(PointDataList list);
		public event PclSendPoints SendPointsEvent;

		public delegate void PclSendPointsBytes(byte[] bs);
		public event PclSendPointsBytes SendPointsBytesEvent;

		public PclPointCloudPointHandle ()
		{
			//SendPoints = new PclSendPoints(
			SendPointsEvent += SendPointCloud;
			genThread = new Thread (new ThreadStart (PclPointsGenarate));
			genThread.Start ();
		}
		public void PclPointsGenarate()
		{
			//int a_ = 10;
			PointData Ori = new PointData ();
			Ori.Coordinate = new Vector3 ();
			Ori.Coordinate.Z = 0;
			float r_ = 50;
			float sph_r = 10;
			float xt = 0;
			int lasterId = 0;
			while (true) {
				xt += 0.2f;
				// 创建一系列point点
				// 创建一个移动的球体
				Ori.Coordinate.X = r_ * (float)Math.Cos((double)xt);
				Ori.Coordinate.Y = r_ *  (float)Math.Sin((double)xt);
				PointDataList l_ = new PointDataList ();

				for(float xita = -0.5f * (float)Math.PI;xita <= 0.5f * (float)Math.PI;xita += 0.1f )
				{
					for (float fai = 0; fai <= 2 * Math.PI; fai += 0.1f) {
						// 循环创建
						PointData p_ = new PointData ();
						p_.LaserId = lasterId;
						p_.Coordinate = new Vector3 ();
						p_.Coordinate.X = Ori.Coordinate.X + sph_r * (float)(Math.Sin ((double)xita) * Math.Cos ((double)fai));
						p_.Coordinate.Y = Ori.Coordinate.Y + sph_r * (float)(Math.Sin ((double)xita) * Math.Sin ((double)fai));
						p_.Coordinate.Z = Ori.Coordinate.Z + sph_r * (float)Math.Cos ((double)xita);
						l_.Points.Add (p_);
						lasterId += 1;
					}
				}

				/*
				PointDataList l_ = new PointDataList ();
				for(int t_ = 0;t_ < 800;t_++){
					PointData p_ = new PointData ();
					p_.LaserId = t_ + a_;
					p_.Coordinate = new Vector3 ();
					p_.Coordinate.X = t_ + a_ ;
					p_.Coordinate.Y = t_ + 1 + a_ ;
					p_.Coordinate.Z = t_ + 2 + a_;
					p_.Radius 		= t_ * 100  + a_;
					p_.Inclination 	= t_ * 100 + 10 + a_;
					p_.Azimuth 		= t_ * 100 + 20 + a_;
					l_.Points.Add (p_);
				}
				a_ += 10;
				*/

				// 进行发送
				// SendPointsEvent(l_);
				SendPointCloud (l_);
				Thread.Sleep (sleepTime);

			}
		}
		public void SendPointCloud(PointDataList list)
		{
			// 讲 point 进行序列化
			if (list.Points.Count < 1) {
				return;
			}
			try{
				Stopwatch sw = new Stopwatch();
				sw.Start();
				// 进行及时
				//using(Google.Protobuf.CodedOutputStream output = new Google.Protobuf.CodedOutputStream())
				byte[] stream = new byte[list.CalculateSize()];
				CodedOutputStream output = new CodedOutputStream(stream);
				list.WriteTo(output);
				PclPointCloudMessage msg = new PclPointCloudMessage();
				msg.length = stream.Length;
				msg.data = stream;
				SendPointsBytesEvent(msg.MsgFormat());
				sw.Stop();
				Console.WriteLine("ListTest " + sw.ElapsedMilliseconds + " 毫秒,数组长度：" + stream.Length);
			}
			catch(Exception e) {
			
			}

		}
	}
}

