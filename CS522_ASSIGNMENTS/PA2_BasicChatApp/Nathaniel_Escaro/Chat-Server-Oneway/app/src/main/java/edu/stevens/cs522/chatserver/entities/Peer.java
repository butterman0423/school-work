package edu.stevens.cs522.chatserver.entities;

import android.os.Parcel;
import android.os.Parcelable;

import java.util.Date;

import edu.stevens.cs522.base.DateUtils;

/**
 * Created by dduggan.
 */

public class Peer implements Parcelable {

    // Will be database key
    public long id;

    public String name;

    // Last time we heard from this peer.
    public Date timestamp;

    // Where we heard from them
    public Double latitude;

    public Double longitude;

    public Peer() {
    }

    @Override
    public String toString() {
        // TODO
        return this.name;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public Peer(Parcel in) {
        // TODO
        this.id = in.readLong();
        this.name = in.readString();
        this.timestamp = DateUtils.readDate(in);
        this.latitude = in.readDouble();
        this.longitude = in.readDouble();
    }

    @Override
    public void writeToParcel(Parcel out, int flags) {
        // TODO
        out.writeLong(this.id);
        out.writeString(this.name);
        DateUtils.writeDate(out, this.timestamp);
        out.writeDouble(this.latitude);
        out.writeDouble(this.longitude);
    }

    public static final Creator<Peer> CREATOR = new Creator<Peer>() {

        @Override
        public Peer createFromParcel(Parcel source) {
            // TODO
            return new Peer(source);
        }

        @Override
        public Peer[] newArray(int size) {
            // TODO
            return new Peer[size];
        }

    };
}
