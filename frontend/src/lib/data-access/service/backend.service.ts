import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { App, List, Item } from '../models';

@Injectable({
  providedIn : 'root'
})
export class BackendService {
  readonly ip: string = "http://localhost:4200";

  constructor( private httpClient: HttpClient ) { }

  loadReminderApp(): Observable<App> {
      return this.httpClient.get<App>(this.ip + "/api/app");
  }

  /*
  getLists(): Observable<List[]> {
      return this.httpClient.get<List[]>(this.ip + "/api/app/lists");
  }
   */

  deleteList(id: number): Observable<void> {
      return this.httpClient.delete<void>(this.ip + "/api/app/lists/" + id);
  }

  postList(newList: List): Observable<List> {
      return this.httpClient.post<List>(this.ip + "/api/app/lists", {"title": newList.title});
  }

  putList(list: List): Observable<List> {
      let body = {
          "title": list.title
      };
      return this.httpClient.put<List>(this.ip + "/api/app/lists/"+list.id, body);
  }

  postItem(listID: number, newItem: Item): Observable<Item> {
    let body = {
      "text": newItem.text,
      "date": "",
      "flagged": false
    };
    return this.httpClient.post<Item>(this.ip + "/api/app/lists/" + listID + "/items", body);
  }

  deleteItem(itemID: number): Observable<void> {
      return this.httpClient.delete<void>(this.ip + "/api/app/items/" + itemID);
  }

  putItem(item: Item): Observable<Item> {
      let body = {
          "text": item.text,
          "date": item.date ? item.date : "",
          "flagged": item.flagged
      };
      return this.httpClient.put<Item>(this.ip + "/api/app/items/" + item.id, body);
  }

  getTodayList(): Observable<List> {
      return this.httpClient.get<List>(this.ip + "/api/app/lists/today");
  }

  getFlaggedList(): Observable<List> {
      return this.httpClient.get<List>(this.ip + "/api/app/lists/flagged");
  }

}
