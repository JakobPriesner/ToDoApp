import {Injectable} from '@angular/core';
import {BehaviorSubject} from 'rxjs';
import {List} from '../data-access/models';

@Injectable({
  providedIn: 'root'
})
export class AppStore {
  private selectedList: BehaviorSubject<List> = new BehaviorSubject({
    title: '',
    items: [],
    selected: false,
    itemCounter: 0
  });
  selectedList$ = this.selectedList.asObservable();

  changeSelectedList(newList: List): void {
      if(this.selectedList.getValue().id != undefined && this.selectedList.getValue().id != newList.id) {
        this.selectedList.getValue().selected = false;
      }
      this.selectedList.next(newList);
  }

}
