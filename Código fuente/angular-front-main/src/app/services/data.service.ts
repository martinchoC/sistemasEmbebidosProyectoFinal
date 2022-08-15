import { Injectable } from '@angular/core';
import { AngularFireDatabase, AngularFireList, AngularFireObject, SnapshotAction } from '@angular/fire/database';
import { Data } from '@angular/router';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class DataService {

  private dataDB:AngularFireObject<Data>;

  constructor(private db:AngularFireDatabase) {
    this.dataDB = this.db.object("/data");
  }

  getData():Observable<SnapshotAction<Data>>
  {
    return this.dataDB.snapshotChanges();
  }
}
